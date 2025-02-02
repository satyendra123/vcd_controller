import React, { useState } from "react";
import "./boomstyle.css";
import openImage from "./openbarrier.png"; // Replace with your actual open image path
import closeImage from "./closebarrier.jpg"; // Replace with your actual close image path
import axios from "axios"
const TabPanel = () => {
  const [backgroundColor, setBackgroundColor] = useState("transparent");
  const [textBackgroundColor, setTextBackgroundColor] = useState("transparent");
  const [status, setStatus] = useState("Open");
  const [image, setImage] = useState(closeImage);

  const handleImageClick = async () => {
    try {
      // Call the API using axios with CSRF token header
      const response = await axios.get("http://127.0.0.1:8000/api/boomendpoints/open/boom", {
        headers: {
          'Cookie': 'csrftoken=j3UvHRKu4xN20JQPzxWOQmbU7PxP3WOw',  // Add the CSRF token here
        }
      });
  console.log(response.data)
      if (response.status === 200) {
        setBackgroundColor("#d1ffd1"); // Set image wrapper background to green
        setTextBackgroundColor("#d1ffd1"); // Set text background to green
        setStatus("Close"); // Set text to "Close"
        setImage(openImage); // Change image to close barrier
  
        // Reset after 30 seconds
        setTimeout(() => {
          setBackgroundColor("transparent");
          setTextBackgroundColor("transparent");
          setStatus("Open");
          setImage(closeImage);
        }, 9000);
      } else {
        console.error("API returned an error");
      }
    } catch (error) {
      console.error("Error calling the API:", error);
    }
  };

  return (
    <div className="tab-panel">
      <div
        className="image-wrapper"
        style={{
          backgroundColor,
          transition: "background-color 0.3s ease",
          display: "flex",
          flexDirection: "column", // Ensure vertical stacking
          alignItems: "center", // Center-align both image and text
        }}
      >
        <img
          src={image}
          alt="Boom Barrier"
          className="boom-barrier"
          onClick={handleImageClick}
          style={{
          marginLeft:"60px",
            cursor: "pointer",
            borderRadius: "5px",
          }}
        />
        <h1
          onClick={handleImageClick} // Make text clickable
          style={{
            margin: "0px 0 0", // Add space above the text
            padding: "5px 10px",
            backgroundColor: textBackgroundColor, // Dynamic background for text
            borderRadius: "5px", // Rounded corners for the text background
            transition: "background-color 0.3s ease",
            cursor: "pointer",
          }}
        >
          {status}
        </h1>
      </div>
    </div>
  );
};

export default TabPanel;
