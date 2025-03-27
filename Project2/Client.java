import OrderManagementApp.*;

import org.omg.CORBA.*;
import org.omg.CosNaming.*;

import java.util.Scanner;

public class Client {
    public static void main(String args[]) {
        try {
            ORB orb = ORB.init(args, null);

            org.omg.CORBA.Object objRef = orb.resolve_initial_references("NameService");
            NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);

            OrderManagement orderRef = OrderManagementHelper.narrow(ncRef.resolve_str("OrderService"));

            Scanner sc = new Scanner(System.in);
            System.out.print("Are you a customer or manager? ");
            String role = sc.nextLine().trim().toLowerCase();

            if (role.equals("customer")) {
                System.out.println("1. View menu & place order");
                System.out.println("2. Check order status");
                System.out.print("Select option (1 or 2): ");
                int option = Integer.parseInt(sc.nextLine());
            
                if (option == 1) {
                    System.out.println(orderRef.viewMenu());
            
                    System.out.print("Enter your username: ");
                    String username = sc.nextLine();
            
                    System.out.print("Enter quantity of Fried Chicken: ");
                    int chicken = Integer.parseInt(sc.nextLine());
            
                    System.out.print("Enter quantity of Cola: ");
                    int cola = Integer.parseInt(sc.nextLine());
            
                    try {
                        String result = orderRef.placeOrder(username, chicken, cola);
                        System.out.println(result);
            
                        Order o = orderRef.checkOrderStatus(username);
                        System.out.println("Order Status:");
                        System.out.println("User: " + o.username);
                        System.out.println("Chicken: " + o.chickenQty);
                        System.out.println("Cola: " + o.colaQty);
                        System.out.println("Total: $" + o.totalPrice);
                    } catch (InvalidOrder ex) {
                        System.out.println("Error: " + ex.message);
                    }
            
                } else if (option == 2) {
                    System.out.print("Enter your username: ");
                    String username = sc.nextLine();
            
                    try {
                        Order o = orderRef.checkOrderStatus(username);
                        System.out.println("Order Status:");
                        System.out.println("User: " + o.username);
                        System.out.println("Chicken: " + o.chickenQty);
                        System.out.println("Cola: " + o.colaQty);
                        System.out.println("Total: $" + o.totalPrice);
                    } catch (InvalidOrder ex) {
                        System.out.println("Error: " + ex.message);
                    }
                } else {
                    System.out.println("Invalid option.");
                }            

            } else if (role.equals("manager")) {
                Order[] orders = orderRef.viewCurrentOrders();
                if (orders.length == 0) {
                    System.out.println("No orders yet.");
                } else {
                    System.out.println("All Current Orders:");
                    for (Order o : orders) {
                        System.out.println("User: " + o.username + " | Chicken: " + o.chickenQty +
                                " | Cola: " + o.colaQty + " | Total: $" + o.totalPrice);
                    }
                }
            } else {
                System.out.println("Invalid role selected.");
            }

            sc.close();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
