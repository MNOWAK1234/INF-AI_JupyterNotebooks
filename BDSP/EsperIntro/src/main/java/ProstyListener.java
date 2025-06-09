import com.espertech.esper.common.client.EventBean;
import com.espertech.esper.runtime.client.EPRuntime;
import com.espertech.esper.runtime.client.EPStatement;
import com.espertech.esper.runtime.client.UpdateListener;

public class ProstyListener implements UpdateListener {
    @Override
    public void update(EventBean[] newEvents, EventBean[] oldEvents, EPStatement epStatement, EPRuntime epRuntime) {
        if (newEvents != null) {
            for (EventBean newEvent : newEvents) {
                System.out.println("ISTREAM : " + newEvent.getUnderlying());
            }
        }
        if (oldEvents != null) {
            for (EventBean oldEvent : oldEvents) {
                System.out.println("RSTREAM : " + oldEvent.getUnderlying());
            }
        }
    }
}
