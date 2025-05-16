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

function WorkersAtWork() {
  const [workersAtWork, setWorkersAtWork] = useState([]);
  const [error, setError] = useState([]);

  const currentDate = new Date().toISOString().split('T')[0]; // Get current date in 'YYYY-MM-DD' format

  useEffect(() => {
    const fetchWorkersAtWork = async () => {
      try {
        const response = await ApiService.getWorkersAtWork(currentDate);
        setWorkersAtWork(response);
        setError(null); // Clear any previous errors
      } catch (error) {
        setError('Failed to get workers at work');
        setWorkersAtWork([]); // Clear workersAtWork on error
      }
    };

    fetchWorkersAtWork();
  }, []); // Empty dependency array ensures this effect runs once on component mount

  return (
    <div>
      {error && <p style={{ color: 'red', textAlign: 'center', marginTop: '10px' }}>{error}</p>}

      <StyledTableTitle>{`Workers at Work on ${currentDate}`}</StyledTableTitle>
      <StyledTableContainer>
        <StyledTable>
          <thead>
            <tr>
              <StyledTableHeader>Name</StyledTableHeader>
            </tr>
          </thead>
          <tbody>
            {workersAtWork.map((worker, index) => (
              <tr key={index}>
                <StyledTableCell>{worker}</StyledTableCell>
              </tr>
            ))}
          </tbody>
        </StyledTable>
      </StyledTableContainer>
    </div>
  );
}

export default WorkersAtWork;
