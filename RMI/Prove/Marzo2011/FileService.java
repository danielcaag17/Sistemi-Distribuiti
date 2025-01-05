package RMI.Prove.Marzo2011;

import java.rmi.*;

public interface FileService extends Remote {
    public void writeToFile(String s) throws RemoteException;    
}
