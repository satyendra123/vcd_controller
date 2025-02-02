import React from "react";
import "./boomstyle.css";

const Footer = () => {
  return (
    <div className="footer-tab-panel">
      <ul className="footer-tabs">
        <li className="active">Log</li>
        <li>Event</li>
        <li>Card Event</li>
      </ul>

      <div className="tab-content">
        <div className="log-container">
          <p className="log-data">Time: 2024-12-09 12:34:56</p>
          <p className="log-data">Gate 1 Open Status: Active</p>
          <p className="log-data">Gate 2 Open Status: Inactive</p>
          {/* Add more log data here */}
        </div>
      </div>
    </div>
  );
};

export default Footer;
