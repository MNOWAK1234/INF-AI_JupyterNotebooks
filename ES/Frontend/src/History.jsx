import React, { useState, useEffect } from 'react';
import ApiService from './ApiService';
import {
  StyledTableTitle,
  StyledTableContainer,
  StyledTable,
  StyledTableHeader,
  StyledTableCell,
} from './Styles';
import 'bootstrap/dist/css/bootstrap.min.css';

function History() {
  const [historyData, setHistoryData] = useState([]);
  const [error, setError] = useState([]);

  useEffect(() => {
    const fetchHistoryData = async () => {
      try {
        const response = await ApiService.getAllData();
        setHistoryData(response);
        setError(null); // Clear any previous errors
      } catch (error) {
        setError('Failed to fetch history data');
        setHistoryData([]); // Clear historyData on error
      }
    };

    fetchHistoryData();
  }, []); // Empty dependency array ensures this effect runs once on component mount

  return (
    <div>
      {error && <p style={{ color: 'red', textAlign: 'center', marginTop: '10px' }}>{error}</p>}

      <StyledTableTitle>Entry History</StyledTableTitle>
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
            {historyData.map((item, index) => (
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

export default History;
