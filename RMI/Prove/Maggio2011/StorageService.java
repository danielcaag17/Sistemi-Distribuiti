package RMI.Prove.Maggio2011;

import java.rmi.*;

public interface StorageService extends Remote {
    public void registry(String nameService, String nameServer) throws RemoteException;
    public String getNameService(String nameServer) throws RemoteException;
}