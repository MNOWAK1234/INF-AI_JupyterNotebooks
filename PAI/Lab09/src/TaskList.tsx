import TaskItem from './TaskItem';
import { Task } from './Task';

interface TaskListProps {
  tasks: Task[];
  hideCompleted: boolean;
  onToggle: (id: number) => void;
}

function TaskList({ tasks, hideCompleted, onToggle }: TaskListProps) {
  const filteredTasks = hideCompleted ? tasks.filter(task => !task.completed) : tasks;

  return (
    <div className="todo-list">
      {tasks.length === 0 && <p>Nothing to do...</p>}
      {filteredTasks.length === 0 && tasks.length > 0 && <p>All tasks are completed!</p>}
      {filteredTasks.map(task => (
        <TaskItem key={task.id} task={task} onToggle={onToggle} />
      ))}
    </div>
  );
}

export default TaskList;
