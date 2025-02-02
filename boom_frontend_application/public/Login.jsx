import { useState } from "react";
import { useNavigate } from "react-router-dom";
import axios from "axios";
import "./Login.css";
import { Container, Row, Col, Form } from "react-bootstrap";
import Landtlogo2 from "./Landtlogo2";

const Login = ({ setIsLoggedIn }) => {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const [error, setError] = useState("");
  const navigate = useNavigate();

  const handleLogin = async (e) => {
    e.preventDefault();
    try {
      const response = await axios.post(
        "http://localhost:8000/api/admin/login",
        {
          username,
          password,
        }
      );

      if (response.data.status === "Success") {
        if (response.data.role === "Admin") {

          localStorage.setItem("token", response.data.token);
          localStorage.setItem("role_name", response.data.role);
          localStorage.setItem("office_id", response.data.office_id);
          setIsLoggedIn(true);

          navigate("/dashboard");
        } else if (response.data.role === "Client") {

          localStorage.setItem("token", response.data.token);
          localStorage.setItem("role_name", response.data.role);
          localStorage.setItem("office_id", response.data.office_id);
          setIsLoggedIn(true);

          navigate("/Tenant/dashboard");
        } else {
          // If role is not Admin or Client, stay on the page and show the error message
          setError("Your role is not authorized to login.");
        }
      } else {
        setError("Login failed. Please check your credentials.");
      }
    } catch (error) {
      setError("An error occurred during login.");
    }
  };

  return (
    <div className="approvedlogin d-flex flex-column justify-content-center align-items-center">
      <Landtlogo2 />
      <Container fluid>
        <Row>
          {/* Left Column */}
          <Col
            sm={12}
            md={4}
            className="d-flex flex-column justify-content-center align-items-center"
            style={{ backgroundColor: "#048AFB", borderRadius: "30px" }}
          >
            <div
              className="p-3"
              style={{ backgroundColor: "#EFF8FF", borderRadius: "20px" }}
            >
              <img src="landtlogo1.svg" alt="Logo" />
            </div>
            <div className="text-center text-white mt-5">
              <img src="computer 1.png" alt="Center Image" />
              <h4 className="mt-4 fw-300">OUR VISION YOUR FUTURE</h4>
              <h6 style={{ opacity: "0.7" }}>Powered by Housys</h6>
            </div>
          </Col>

          {/* Right Column */}
          <Col
            sm={12}
            md={8}
            className="d-flex justify-content-center align-items-center"
          >
            <Form
              className="w-100 p-4"
              style={{ maxWidth: "500px" }}
              onSubmit={handleLogin}
            >
              <div
                className="d-flex justify-content-between align-items-center mb-4"
                style={{
                  background:
                    "linear-gradient(to left, #E5F3FF 50%, #048AFB 50%)",
                  height: "30px",
                  borderRadius: "10px",
                }}
              >
                <h5 className="text-white mx-3">Login</h5>
                {/* <h5 className="text-primary mx-3">Register</h5> */}
              </div>

              <div className="form-group mb-4">
                <label htmlFor="exampleInputEmail1">User Name</label>
                <input
                  type="text"
                  className="form-control mt-2 py-2"
                  style={{
                    borderRadius: "14px",
                    border: "1px solid #058AFB",
                    background: "white",
                  }}
                  id="exampleInputEmail1"
                  placeholder="Enter username"
                  value={username}
                  onChange={(e) => setUsername(e.target.value)}
                />
              </div>
              <div className="form-group mb-4">
                <label htmlFor="exampleInputPassword1">Password</label>
                <input
                  type="password"
                  className="form-control mt-2 py-2"
                  style={{
                    borderRadius: "14px",
                    border: "1px solid #058AFB",
                    background: "white",
                  }}
                  id="exampleInputPassword1"
                  placeholder="Password"
                  value={password}
                  onChange={(e) => setPassword(e.target.value)}
                />
              </div>
              {/* <div className="form-check d-flex align-items-center justify-content-between mb-4">
                <div className="d-flex align-items-center">
                  <input
                    type="checkbox"
                    className="form-check-input"
                    id="exampleCheck1"
                  />
                  <label
                    className="form-check-label ml-2"
                    htmlFor="exampleCheck1"
                  >
                    Remember me
                  </label>
                </div>
                <a href="#" className="text-muted">
                  Forgot password
                </a>
              </div> */}

              {error && <p className="text-danger">{error}</p>}

              <button
                type="submit"
                className="btn btn-primary w-100"
                style={{
                  borderRadius: "30px",
                }}
              >
                Login
              </button>
            </Form>
          </Col>
        </Row>
      </Container>
    </div>
  );
};

export default Login;
