package RMI.Prove.Maggio2011;

import java.rmi.Naming;

public class ServerB {
    public static void main(String[] args) {
        try {
            Naming.rebind("//localhost/ReadServiceImpl", new ReadServiceImpl());
            StorageService servicioC =  (StorageService) Naming.lookup("//localhost/StorageImpl");
            servicioC.registry("ReadServiceImpl", "ServerB");
        } catch (Exception e) {
            System.out.println(e);
        }
    }
}
