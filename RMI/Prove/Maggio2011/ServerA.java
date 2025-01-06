package RMI.Prove.Maggio2011;

import java.rmi.Naming;

public class ServerA {
        public static void main(String[] args) {
        try {
            Naming.rebind("//localhost/WriteServiceImpl", new WriteServiceImpl());
            StorageService storageImpl =  (StorageService) Naming.lookup("//localhost/StorageServiceImpl");
            storageImpl.registry("WriteServiceImpl", "ServerA");
        } catch (Exception e) {
            System.out.println(e);
        }
    }
}
