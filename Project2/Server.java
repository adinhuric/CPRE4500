import OrderManagementApp.*;

import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.CosNaming.*;

import java.util.*;

class OrderManagementImpl extends OrderManagementPOA {
    private ORB orb;
    private Map<String, Order> orderMap = new HashMap<>();

    public void setORB(ORB orb_val) {
        orb = orb_val;
    }

    public String viewMenu() {
        return "MENU:\n1. Fried Chicken - $5\n2. Cola - $1";
    }

    public String placeOrder(String username, int chickenQty, int colaQty) throws InvalidOrder {
        if (chickenQty < 0 || colaQty < 0 || orderMap.containsKey(username)) {
            throw new InvalidOrder("Invalid order: quantity cannot be negative or user already ordered.");
        }

        double total = chickenQty * 5.0 + colaQty * 1.0;
        Order order = new Order(username, chickenQty, colaQty, total);
        orderMap.put(username, order);
        return "The order is received";
    }

    public Order checkOrderStatus(String username) throws InvalidOrder {
        if (!orderMap.containsKey(username)) {
            throw new InvalidOrder("No order found for username: " + username);
        }
        return orderMap.get(username);
    }

    public Order[] viewCurrentOrders() {
        Collection<Order> values = orderMap.values();
        return values.toArray(new Order[0]);
    }
}

public class Server {
    public static void main(String args[]) {
        try {
            ORB orb = ORB.init(args, null);

            POA rootpoa = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
            rootpoa.the_POAManager().activate();

            OrderManagementImpl orderImpl = new OrderManagementImpl();
            orderImpl.setORB(orb);

            org.omg.CORBA.Object ref = rootpoa.servant_to_reference(orderImpl);
            OrderManagement href = OrderManagementHelper.narrow(ref);

            org.omg.CORBA.Object objRef = orb.resolve_initial_references("NameService");
            NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);

            NameComponent path[] = ncRef.to_name("OrderService");
            ncRef.rebind(path, href);

            System.out.println("Server ready and waiting...");
            orb.run();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
