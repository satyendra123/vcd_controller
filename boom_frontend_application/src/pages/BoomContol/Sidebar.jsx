import React from "react";
import "./boomstyle.css";
import { useNavigate } from "react-router-dom";
import axios from "axios";

const Sidebar = () => {
  const navigate = useNavigate(); // Initialize navigate
  // Sign-out function
  const signout = async () => {
    // alert("token")

    try {
      const token = localStorage.getItem("user-info"); // Fetch token from localStorage

      if (!token ) {
        console.error("Missing token or CSRF token.");
        return;
      }
      else{
alert(token)}
      // Axios request
      const response = await axios.post(
        "http://localhost:8000/api/auth/logout/",
        {}, // POST body is empty
        {
          headers: {
            Authorization: `Token ${token}`, // Include CSRF token in cookie header
          },
        }
      );

      if (response.status === 200) {
        console.log("Sign-out successful");

        // Clear the token and navigate to the login page
        localStorage.removeItem("user-info");
        navigate("/"); // Navigate to login page
        window.location.reload(); // Optional: Reload the page
      }
    } catch (error) {
      console.error("Error signing out:", error);
    }
  };

  return (
    <div className="sidebar">
      <ul>
        <li>Gate Management</li>
        <li>Card Holder</li>
        <li>Card Record</li>
        <li>Alarm Event</li>
        <li>User</li>
        <li
          
            className="sidebar-link"
            onClick={signout} // Call the SignOut function on click
           style={{ cursor: "pointer" }}
          >
            SignOut
          
        </li>
        <li>
          <a
            href="https://www.houstonsystem.com/"
            target="_blank"
            rel="noopener noreferrer"
            className="sidebar-link"
          >
            About
          </a>
        </li>
      </ul>
    </div>
  );
};

export default Sidebar;
