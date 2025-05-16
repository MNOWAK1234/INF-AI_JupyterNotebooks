import React from 'react';
import {
  StyledBackground,
  StyledTitle,
  StyledNavBar,
  StyledNavLink,
} from './Styles';
import 'bootstrap/dist/css/bootstrap.min.css';
import { BrowserRouter as Router, Routes, Route} from 'react-router-dom';
import Home from './Home';
import WorkerData from './WorkerData';
import DataByTimestamp from './DataByTimestamp';
import DataByWorkerAndTimestamps from './DataByWorkerAndTimestamps';
import EntryCount from './EntryCount';
import TodayWorkers from './TodayWorkers';
import History from './History';

function App() {

  return (
    <Router>
      <StyledBackground>
        <div>
          <StyledTitle>Administration Panel</StyledTitle>
          <StyledNavBar>
            <StyledNavLink to="/">Home</StyledNavLink>
            <StyledNavLink to="/workerdata">Worker Data</StyledNavLink>
            <StyledNavLink to="/databytimestamp">Entries By Timestamp</StyledNavLink>
            <StyledNavLink to="/databyworkerandtimestamps">Entries of Worker by Timestamps</StyledNavLink>
            <StyledNavLink to="/CurrentWorkers">Working Today</StyledNavLink>
            <StyledNavLink to="/EntryCount">Statistics</StyledNavLink>
            <StyledNavLink to="/AllData">History</StyledNavLink>
          </StyledNavBar>
          <Routes>
            <Route path="/" element={<Home />} />
            <Route path="/workerdata" element={<WorkerData />} />
            <Route path="/databytimestamp" element={<DataByTimestamp />} />
            <Route path="/databyworkerandtimestamps" element={<DataByWorkerAndTimestamps />} />
            <Route path="/CurrentWorkers" element={<TodayWorkers />} />
            <Route path="/EntryCount" element={<EntryCount />} />
            <Route path="/AllData" element={<History />} />
          </Routes>
        </div>
      </StyledBackground>
    </Router>
  );
}

export default App;
