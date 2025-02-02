import React from "react";
import TableView from "./TableView";
import TabPanel from "./TabPanel";
import Footer from "./Footer"
import "./boomstyle.css";

const MainContent = () => {
  return (
    <div className="main-content">
      <TableView />
      <TabPanel />
      <Footer/>
    </div>
  );
};

export default MainContent;
