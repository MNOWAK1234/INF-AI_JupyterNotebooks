import { useState } from 'react';
import './App.css';
import Header from './Header';
import TaskList from './TaskList';
import AddTask from './AddTask';
import HideCompletedToggle from './HideCompletedToggle';
import { Task } from './Task';

function App() {
  const [tasks, setTasks] = useState<Task[]>([]);
  const [hideCompleted, setHideCompleted] = useState(false);

  const toggleTask = (id: number) => {
    setTasks(tasks.map(task => {
      if (task.id === id) {
        return { ...task, completed: !task.completed };
      }
      return task;
    }));
  };

  const handleHideCompleted = () => {
    setHideCompleted(!hideCompleted);
  };

  return (
    <div className="app-container">
      <Header />
      <div className="separator"></div>
      <HideCompletedToggle hideCompleted={hideCompleted} onToggle={handleHideCompleted} />
      <TaskList tasks={tasks} hideCompleted={hideCompleted} onToggle={toggleTask} />
      <div className="separator"></div>
      <AddTask tasks={tasks} setTasks={setTasks} />
    </div>
  );
}

export default App;
