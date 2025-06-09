import com.espertech.esper.common.client.EventBean;
import com.espertech.esper.runtime.client.EPRuntime;
import com.espertech.esper.runtime.client.EPStatement;
import com.espertech.esper.runtime.client.UpdateListener;

public class ProstyListener implements UpdateListener {
    @Override
    public void update(EventBean[] newEvents, EventBean[] oldEvents,
                       EPStatement statement, EPRuntime runtime) {
        int now = (int)(System.currentTimeMillis() % 10000);
        if (newEvents != null) {
            for (EventBean newEvent : newEvents) {
                System.out.println(now + ": ISTREAM : " + newEvent.getUnderlying());
            }
        }
        if (oldEvents != null) {
            for (EventBean oldEvent : oldEvents) {
                System.out.println(now + ": RSTREAM : " + oldEvent.getUnderlying());
            }
        }
    }
}
