import styled from 'styled-components';
import { Link } from 'react-router-dom';

export const StyledTableContainer = styled.div`
  background: linear-gradient(135deg, #667eea, #764ba2);
  min-height: 10vh;
  display: flex;
  align-items: center;
  justify-content: center;
  padding: 2% 10%; /* Add some padding to the sides */
`;

export const StyledTable = styled.table`
  width: 100%;
  background-color: white;
  border-radius: 10px;
  box-shadow: 0px 0px 20px rgba(0, 0, 0, 0.1);
  overflow-x: auto;
  table-layout: fixed;
`;

export const StyledTableCell = styled.td`
  padding: 15px;
  text-align: left;
`;

export const StyledTableHeader = styled.th`
  padding: 15px;
  text-align: left;
  background-color: #f2f2f2;
`;

export const StyledTitle = styled.h1`
  text-align: center;
  color: white;
  font-size: 2.5em;
  padding-top: 2%;
  background-color: #333;
  margin: 0;
`;

export const StyledTableTitle = styled.h2`
  text-align: center;
  color: white;
  margin-top: 100px;
  margin-bottom: 10px;
`;

export const StyledBackground = styled.div`
  background: linear-gradient(135deg, #667eea, #764ba2);
  min-height: 100vh;
`;

export const StyledNavBar = styled.nav`
  background-color: #333;
  padding: 20px;
  display: flex;
  justify-content: center;
`;

export const StyledNavLink = styled(Link)`
  color: white;
  text-decoration: none;
  margin: 0 20px;
  cursor: pointer;
  transition: color 0.3s;

  &:hover {
    color: lightgray;
  }
`;

export const inputButtonContainerStyle = {
  display: 'flex',
  flexDirection: 'column',
  alignItems: 'center',
  margin: '20px',
};

export const inputStyle = {
  marginBottom: '10px',
  marginLeft: '10px',
  marginRight: '10px',
  padding: '10px',
  borderRadius: '5px',
  border: '1px solid #ccc',
  width: '200px',
};