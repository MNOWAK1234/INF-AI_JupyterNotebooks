import React, { useState } from 'react';
import ApiService from './ApiService';
import {
  StyledTableTitle,
  StyledTableContainer,
  StyledTable,
  StyledTableHeader,
  StyledTableCell,
  inputButtonContainerStyle,
  inputStyle,
} from './Styles';
import 'bootstrap/dist/css/bootstrap.min.css';

function WorkerData() {
  const [workerName, setWorkerName] = useState('');
  const [workerEntries, setWorkerEntries] = useState([]);
  const [error, setError] = useState([]);

  const handleInputChange = (event) => {
    setWorkerName(event.target.value);
  };

  const handleFetchData = async () => {
    try {
      const response = await ApiService.getWorkerData(workerName);
      setWorkerEntries(response);
      setError(null); // Clear any previous errors
    } catch (error) {
      setError('Failed to fetch worker data');
      setWorkerEntries([]); // Clear workerEntries on error
    }
  };

  return (
    <div>
      <div style={inputButtonContainerStyle}>
        <label>
          Worker's Name:
          <input
            type="text"
            value={workerName}
            onChange={handleInputChange}
            style={inputStyle}
          />
        </label>
        <button onClick={handleFetchData}>Search</button>
      </div>

      {error && <p style={{ color: 'red', textAlign: 'center', marginTop: '10px' }}>{error}</p>}


      <StyledTableTitle>{workerName ? `Worker Data for ${workerName}` : 'Worker Data'}</StyledTableTitle>
      <StyledTableContainer>
        <StyledTable>
          <thead>
            <tr>
              <StyledTableHeader>ID</StyledTableHeader>
              <StyledTableHeader>Name</StyledTableHeader>
              <StyledTableHeader>Timestamp</StyledTableHeader>
              <StyledTableHeader>Status</StyledTableHeader>
            </tr>
          </thead>
          <tbody>
            {workerEntries.map((item, index) => (
              <tr key={index}>
                <StyledTableCell>{item[0]}</StyledTableCell>
                <StyledTableCell>{item[1]}</StyledTableCell>
                <StyledTableCell>{item[2]}</StyledTableCell>
                <StyledTableCell>{item[3]}</StyledTableCell>
              </tr>
            ))}
          </tbody>
        </StyledTable>
      </StyledTableContainer>
    </div>
  );
}

export default WorkerData;
