// EntryCount.js

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

function EntryCount() {
  const [startTime, setStartTime] = useState('');
  const [endTime, setEndTime] = useState('');
  const [entryCountData, setEntryCountData] = useState([]);
  const [error, setError] = useState([]);

  const handleStartTimeChange = (event) => {
    setStartTime(event.target.value);
  };

  const handleEndTimeChange = (event) => {
    setEndTime(event.target.value);
  };

  const handleFetchData = async () => {
    try {
      const response = await ApiService.countAppearancesByTimestamp(startTime, endTime);
      setEntryCountData(response);
      setError(null); // Clear any previous errors
    } catch (error) {
      setError('Failed to count appearances by timestamp');
      setEntryCountData([]); // Clear entryCountData on error
    }
  };

  return (
    <div>
      <div style={inputButtonContainerStyle}>
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
        {startTime && endTime
          ? `Appearances Count from ${startTime.replace('T', ' ')} to ${endTime.replace('T', ' ')}`
          : 'Entry Count'}
      </StyledTableTitle>
      <StyledTableContainer>
        <StyledTable>
          <thead>
            <tr>
              <StyledTableHeader>Worker Name</StyledTableHeader>
              <StyledTableHeader>Appearance Count</StyledTableHeader>
            </tr>
          </thead>
          <tbody>
            {entryCountData.map((item, index) => (
              <tr key={index}>
                <StyledTableCell>{item[0]}</StyledTableCell>
                <StyledTableCell>{item[1]}</StyledTableCell>
              </tr>
            ))}
          </tbody>
        </StyledTable>
      </StyledTableContainer>
    </div>
  );
}

export default EntryCount;
