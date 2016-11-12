from FlightManager import Task 
import numpy

class SimpleCountdown( Task ):
    
    def activate(self, manager):
        Task.activate(self,manager)
        print("10 Seconds til Launch!")
    def tick(self, manager):
        if (manager.ticks-self.tickActivated)*manager.ticklength >= 10:
            manager.vessel.stage()
            return True 
        return False
        
class KeepUpwards( Task ):
    def tick(self, manager):
        vessel = manager.vessel.rawVessel
        vessel_direction = vessel.direction(vessel.surface_reference_frame)
        horizon_direction = (0, vessel_direction[1], vessel_direction[2])
        print("Direction: "+str(vessel_direction))