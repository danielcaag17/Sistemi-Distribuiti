package RMI.Prove.Marzo2011;

import java.rmi.*;

public interface ResourceManager {
    boolean requestAccess(String clientId) throws RemoteException;
    void releaseAccess(String clientId) throws RemoteException;
}
