import socket
import datetime
import time
import mysql.connector
import threading

CONFIG_FILE = "config.txt"
LOG_FILE = "Entry_Boom_services.txt"
INITIAL_MESSAGE = "|ENTRY%"

def read_config():
    config = {}
    with open(CONFIG_FILE, 'r') as file:
        for line in file:
            line = line.strip()
            if "=" in line:
                key, value = line.split("=", 1)
                config[key] = value
    return config

def parse_db_config(db_path):
    db_config = {}
    items = db_path.split(";")
    for item in items:
        if "=" in item:
            key, value = item.split("=")
            db_config[key] = value
    return db_config

def log_message(message):
    with open(LOG_FILE, 'a') as log:
        timestamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        log.write(f"{timestamp}: {message}\n")

def send_message(client_socket, message):
    try:
        client_socket.sendall(message.encode())
        log_message(f"Sent message: {message}")
    except Exception as e:
        log_message(f"Failed to send message: {e}")

def check_entry_boom(client_socket, db_config):
    last_status = None
    while True:
        try:
            conn = mysql.connector.connect(**db_config)
            cursor = conn.cursor()

            query = "SELECT entryboom FROM boomsig1"
            cursor.execute(query)

            result = cursor.fetchone()
            if result:
                entry_boom_status = result[0]
                if entry_boom_status == 'Y':
                    if last_status != 'Y':
                        log_message("Entry boom is open")
                        send_message(client_socket, INITIAL_MESSAGE)
                        time.sleep(3)  # Simulate wait time
                        
                        # Update the exitboom status back to 'N'
                        cursor.execute("UPDATE boomsig1 SET entryboom = 'N'")
                        conn.commit()
                        log_message("Entry boom status reset to 'N' after 3 seconds")
                last_status = entry_boom_status

            cursor.close()
            conn.close()
            time.sleep(3)  # Wait before next check
        except mysql.connector.Error as err:
            log_message(f"Database error: {err}")
            time.sleep(3)  # Wait before retrying on error
        except Exception as e:
            log_message(f"Unexpected error: {e}")
            break  # Exit the loop if an unexpected error occurs

def connect_to_db(db_config):
    """Tries to connect to the database, logs errors, and retries on failure."""
    db_conn = None
    while db_conn is None:
        try:
            db_conn = mysql.connector.connect(**db_config)
            log_message("Database connection established.")
        except mysql.connector.Error as err:
            log_message(f"Database connection error: {err}")
            log_message("Retrying in 3 seconds...")
            time.sleep(3)  # Wait before retrying
    return db_conn

def main():
    config = read_config()

    SERVER_IP = config['Server_IP']
    SERVER_PORT = int(config['Port'])
    DB_PATH = config['DbPath']

    db_config = parse_db_config(DB_PATH)

    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        client_socket.connect((SERVER_IP, SERVER_PORT))
        log_message(f"Connected to {SERVER_IP}:{SERVER_PORT}")

        # Start a thread to monitor exit boom status and interact with the database
        thread = threading.Thread(target=check_entry_boom, args=(client_socket, db_config))
        thread.daemon = True
        thread.start()

        while True:
            data = client_socket.recv(1024)
            if data:
                decoded_data = data.decode('utf-8')
                if "|HLT%" in decoded_data:
                    log_message(f"Received health packet: {decoded_data}")

    except ConnectionRefusedError:
        log_message(f"Connection to {SERVER_IP}:{SERVER_PORT} was refused.")
    except TimeoutError:
        log_message("Connection timed out. Check IP and port.")
    except Exception as e:
        log_message(f"An error occurred: {e}")
    finally:
        # Safely close the client socket
        if client_socket:
            client_socket.close()

if __name__ == "__main__":
    main()
