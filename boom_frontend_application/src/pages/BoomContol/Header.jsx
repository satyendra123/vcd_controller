import React from "react";
import "./boomstyle.css";
import logo from "./logo-bg.png";

const Header = () => {
  return (
    <div className="header">
      <div className="logo-container">
        <img
          src={logo}
          alt="Boom Barrier"
          className="boom-barrier"
          style={{
            cursor: "pointer",
            borderRadius: "5px",
            height: "50px", // Adjust the size of the logo if necessary
          }}
        />
      </div>
      <div className="menu">
        <span>System</span>
        <span>Manage</span>
        <span>Access Control</span>
        <span>Time Attendance</span>
        <span>Record</span>
        <span>Help</span>
      </div>
    </div>
  );
};

export default Header;
