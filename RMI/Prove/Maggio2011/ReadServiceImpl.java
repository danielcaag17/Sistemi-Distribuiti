package RMI.Prove.Maggio2011;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.rmi.*;
import java.rmi.server.UnicastRemoteObject;

public class ReadServiceImpl extends UnicastRemoteObject implements ReadService {
    public ReadServiceImpl() throws RemoteException {
        super();
    }

    @Override
    public String readFile(String Path) throws RemoteException {
        try {
            return Files.readString(Paths.get(Path));
        } catch (IOException e) {
            System.out.println(e);
        }
        return null;
    }    
}
