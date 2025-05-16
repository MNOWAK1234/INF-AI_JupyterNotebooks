import { Task } from './Task';

interface TaskItemProps {
  task: Task;
  onToggle: (id: number) => void;
}

function TaskItem({ task, onToggle }: TaskItemProps) {
  return (
    <div className="todo-item">
      <input
        type="checkbox"
        checked={task.completed}
        onChange={() => onToggle(task.id)}
      />
      <span style={{ textDecoration: task.completed ? 'line-through' : 'none' }}>{task.text}</span>
    </div>
  );
}

export default TaskItem;
