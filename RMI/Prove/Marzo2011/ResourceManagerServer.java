package RMI.Prove.Marzo2011;

import java.rmi.*;

public class ResourceManagerServer {
    public static void main(String[] args) {
        try {
            Naming.rebind("//localhost/ResourceManagerImpl", new ResourceManagerImpl());
        } catch (Exception e) {
            System.out.println(e);
        }
    }
}
