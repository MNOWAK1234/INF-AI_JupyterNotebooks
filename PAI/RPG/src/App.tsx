import { useState, useEffect, useRef } from 'react';
import './App.css';

function App() {
  const [story, setStory] = useState<any>(null);
  const [currentEventIndex, setCurrentEventIndex] = useState<number>(0);
  const [selectedOptionId, setSelectedOptionId] = useState<number | null>(null);
  const buttonRefs = useRef<Array<HTMLButtonElement | null>>([]);

  useEffect(() => {
    const fetchStory = async () => {
      try {
        const response = await fetch('./story.json');
        const data = await response.json();
        setStory(data);
      } catch (error) {
        console.error('Error fetching story:', error);
      }
    };

    fetchStory();
  }, []);

  const handleOptionSelect = (outcomeId: number) => {
    const currentEvent = story.events.find((event: any) => event.id === currentEventIndex);
    const selectedOption = currentEvent.options.find((option: any) => option.id === outcomeId);
    const randomValue = Math.random();

    let nextEventId;
    // Determine the next event based on the random value and probabilities
    if (randomValue < selectedOption.outcomes[0].probability) {
      nextEventId = selectedOption.outcomes[0].outcomeId;
    } else {
      nextEventId = selectedOption.outcomes[1].outcomeId;
    }
    
    setCurrentEventIndex(nextEventId);

    // Reset selected option
    setSelectedOptionId(null);

    // Blur all buttons
    buttonRefs.current.forEach((ref) => {
      if (ref) {
        ref.blur();
      }
    });
  };

  if (!story) {
    return <div>Loading...</div>;
  }

  const currentEvent = story.events.find((event: any) => event.id === currentEventIndex);
  const totalEvents = story.events.length;
  const progressPercentage = ((currentEventIndex + 1) / (totalEvents - 4)) * 100;

  return (
    <div className="app-container">
      <div className="container">
        {currentEvent && (
          <div>
            <h1 className="story-text">{currentEvent.text}</h1>
            <div className="options">
              {currentEvent.options.map((option: any, index: number) => (
                <button
                  ref={(ref) => (buttonRefs.current[index] = ref)}
                  key={option.id}
                  onClick={() => handleOptionSelect(option.id)}
                  disabled={selectedOptionId !== null} // Disable button if an option is already selected
                >
                  {option.text}
                </button>
              ))}
            </div>
            <hr className="line" />
            <div className="progress-bar">
              <div className="progress" style={{ width: `${progressPercentage}%` }}></div>
            </div>
          </div>
        )}
      </div>
    </div>
  );
}

export default App;
