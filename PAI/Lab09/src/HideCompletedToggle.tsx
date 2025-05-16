interface HideCompletedToggleProps {
  hideCompleted: boolean;
  onToggle: () => void;
}

function HideCompletedToggle({ hideCompleted, onToggle }: HideCompletedToggleProps) {
  return (
    <div className="hide-completed">
      <input
        type="checkbox"
        checked={hideCompleted}
        onChange={onToggle}
      />
      <label>Hide completed tasks</label>
    </div>
  );
}

export default HideCompletedToggle;
