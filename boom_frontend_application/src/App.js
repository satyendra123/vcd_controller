import "./App.css";
import React, { useState, useEffect } from "react";
import { BrowserRouter as Router, Route, Routes } from "react-router-dom";
import Login from "./pages/Login/Login";
import Header from "./pages/BoomContol/Header";
import Sidebar from "./pages/BoomContol/Sidebar";
import MainContent from "./pages/BoomContol/MainContent";
import "./pages/BoomContol/boomstyle.css";

function App() {
  const [isLoggedIn, setIsLoggedIn] = useState(false);
  const [tokenChecked, setTokenChecked] = useState(false);

  useEffect(() => {
    const token = localStorage.getItem("user-info");
    setIsLoggedIn(!!token);
    setTokenChecked(true);
  }, []);

  if (!tokenChecked) {
    return <div>Loading...</div>;
  }

  if (!isLoggedIn) {
    return (
      <Router>
        <Routes>
          <Route path="/" element={<Login />} />
        </Routes>
      </Router>
    );
  } else {
    return (
      <Router>
        <Routes>
          {/* Define a protected route for the main layout */}
          <Route
            path="/"
            element={
              <div className="app">
                <Header />
                <div className="main">
                  <Sidebar />
                  <MainContent /> {/* Ensure this is a JSX element */}
                </div>
              </div>
            }
          />
        </Routes>
      </Router>
    );
  }
}

export default App;
