package RMI.Prove.Marzo2011;

import java.rmi.Naming;

public class FileServiceServer {
     public static void main(String[] args) {
        try {
            Naming.rebind("//localhost/FileServiceImpl", new FileServiceImpl("archivo_remoto.txt"));
        } catch (Exception e) {
            System.out.println(e);
        }
    }
}
