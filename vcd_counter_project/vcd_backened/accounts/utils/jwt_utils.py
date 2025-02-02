import jwt
import datetime
from django.conf import settings

# Secret key for encoding and decoding the token
SECRET_KEY = settings.SECRET_KEY

# Function to generate JWT token
def generate_jwt(user):
    expiration = datetime.datetime.utcnow() + datetime.timedelta(hours=1)  # Token expires in 1 hour
    payload = {
        'user_id': user.id,
        'username': user.username,
        'exp': expiration
    }
    token = jwt.encode(payload, SECRET_KEY, algorithm='HS256')
    return token

# Function to decode and verify JWT token
def decode_jwt(token):
    try:
        payload = jwt.decode(token, SECRET_KEY, algorithms=['HS256'])
        return payload
    except jwt.ExpiredSignatureError:
        raise Exception("Token has expired")
    except jwt.InvalidTokenError:
        raise Exception("Invalid token")
