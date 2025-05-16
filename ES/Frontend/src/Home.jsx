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

function Home() {
  const [data, setData] = useState([]);
  const [entriesToShow, setEntriesToShow] = useState(4);
  const [totalEntries, setTotalEntries] = useState(0);

  const fetchData = async () => {
    try {
      const allData = await ApiService.getAllData();
      setData(allData);
      setTotalEntries(allData.length);
    } catch (error) {
      console.error('Failed to fetch data:', error);
    }
  };

  useEffect(() => {
    const interval = setInterval(() => {
      fetchData();
    }, 10000);

    return () => clearInterval(interval);
  }, []);

  useEffect(() => {
    fetchData();
  }, []);

  const loadMoreEntries = () => {
    setEntriesToShow(entriesToShow + 2);
  };

  return (
    <div>
      <StyledTableTitle>Access Logs</StyledTableTitle>
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
            {data.slice(0, entriesToShow).map((item, index) => (
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
      {entriesToShow < totalEntries && (
        <div style={{ textAlign: 'center', marginTop: '20px' }}>
          <button onClick={loadMoreEntries}>Get 2 More</button>
        </div>
      )}
    </div>
  );
}

export default Home;
