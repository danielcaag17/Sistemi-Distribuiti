package RMI.Prove.Maggio2011;

import java.rmi.Naming;

public class ServerC {
     public static void main(String[] args) {
        try {
            Naming.rebind("//localhost/StorageServiceImpl", new StorageServiceImpl());
        } catch (Exception e) {
            System.out.println(e);
        }
    }
}
