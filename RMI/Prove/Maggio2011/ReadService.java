package RMI.Prove.Maggio2011;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface ReadService extends Remote {
    public String readFile(String Path) throws RemoteException;
}