import React, { useState } from 'react';
import { Task } from './Task'; 

interface AddTaskProps {
  tasks: Task[];
  setTasks: React.Dispatch<React.SetStateAction<Task[]>>;
}

function AddTask({ tasks, setTasks }: AddTaskProps) {
  const [newTask, setNewTask] = useState('');

  const addTask = () => {
    if (newTask.trim() !== '') {
      const newRow: Task = {
        id: tasks.length === 0 ? 1 : tasks[tasks.length - 1].id + 1,
        text: newTask.trim(),
        completed: false
      };
      setTasks([...tasks, newRow]);
      setNewTask('');
    }
  };

  return (
    <div className="add-task">
      <input
        type="text"
        value={newTask}
        onChange={(e) => setNewTask(e.target.value)}
        placeholder="Enter new task"
      />
      <button onClick={addTask}>Add</button>
    </div>
  );
}

export default AddTask;
