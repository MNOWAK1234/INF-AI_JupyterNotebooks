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

function DataByWorkerAndTimestamp() {
  const [workerName, setWorkerName] = useState('');
  const [startTime, setStartTime] = useState('');
  const [endTime, setEndTime] = useState('');
  const [dataEntries, setDataEntries] = useState([]);
  const [error, setError] = useState([]);

  const handleWorkerNameChange = (event) => {
    setWorkerName(event.target.value);
  };

  const handleStartTimeChange = (event) => {
    setStartTime(event.target.value);
  };

  const handleEndTimeChange = (event) => {
    setEndTime(event.target.value);
  };

  const handleFetchData = async () => {
    try {
      const response = await ApiService.getDataByWorkerAndTimestamps(workerName, startTime, endTime);
      setDataEntries(response);
      setError(null); // Clear any previous errors
    } catch (error) {
      setError('Failed to get data by worker and timestamps');
      setDataEntries([]); // Clear dataEntries on error
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
            onChange={handleWorkerNameChange}
            style={inputStyle}
          />
        </label>
        <label>
          Start Timestamp:
          <input
            type="datetime-local"
            value={startTime}
            onChange={handleStartTimeChange}
            style={inputStyle}
          />
        </label>
        <label>
          End Timestamp:
          <input
            type="datetime-local"
            value={endTime}
            onChange={handleEndTimeChange}
            style={inputStyle}
          />
        </label>
        <button onClick={handleFetchData}>Search</button>
      </div>

      {error && <p style={{ color: 'red', textAlign: 'center', marginTop: '10px' }}>{error}</p>}

      <StyledTableTitle>
        {workerName && startTime && endTime
          ? `Data for ${workerName} from ${startTime.replace('T', ' ')} to ${endTime.replace('T', ' ')}`
          : 'Data By Worker and Timestamps'}
      </StyledTableTitle>
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
            {dataEntries.map((item, index) => (
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

export default DataByWorkerAndTimestamp;
