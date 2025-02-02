import React from "react";
import "./boomstyle.css";

const TableView = () => {
  return (
    <div className="table-view">
      <table>
        <thead>
          <tr>
            <th>Interface</th>
            <th>Status</th>
            <th>Model No.</th>
            <th>Communication</th>
            <th>Serial No.</th>
            <th>IP Address</th>
            <th>Port</th>
            <th>Group</th>
            <th>Area</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td>Entry Gate</td>
            <td>Active</td>
            <td>Gate No.1</td>
            <td>1</td>
            <td>192.168.1.107</td>
            <td>8000</td>
            <td>1</td>
            <td>1</td>
            <td>Basement 1</td>
          </tr>
        </tbody>
      </table>
    </div>
  );
};

export default TableView;
