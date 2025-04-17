from os import path
import sys
import json

from pymobiledevice3.lockdown import create_using_usbmux
from pymobiledevice3.usbmux import select_devices_by_connection_type
from pymobiledevice3.services.house_arrest import HouseArrestService


def get_lockdown_client(usbmux_address=None):
  devices = select_devices_by_connection_type(connection_type='USB', usbmux_address=usbmux_address)
  # TODO: Handle multiple devices and issues with preparing the device
  if len(devices) == 1:
    return create_using_usbmux(usbmux_address=usbmux_address)
  elif len(devices) > 1:
    raise Exception("Multiple devices found")
  else:
    raise Exception("No device found")

MINECRAFT_BUNDLE_ID = "com.mojang.minecraftpe"
MOJANG_ROOT_PATH = "/Documents/games/com.mojang/"
DEV_BEHAVIOR_PACKS_PATH = MOJANG_ROOT_PATH + "development_behavior_packs/"
DEV_RESOURCE_PACKS_PATH = MOJANG_ROOT_PATH + "development_resource_packs/"
DEV_SKIN_PACKS_PATH = MOJANG_ROOT_PATH + "development_skin_packs/"

class MinepackClient:
  def __init__(self, usbmux_address=None):
    self.lockdown_client = get_lockdown_client(usbmux_address)
    self.house_arrest = HouseArrestService(lockdown=self.lockdown_client, bundle_id=MINECRAFT_BUNDLE_ID, documents_only=True)
    # State variables
    self.addon_name = None
    self.local_behavior_packs_path = None
    self.local_resource_packs_path = None
    self.local_skin_packs_path = None
    self.is_configured = False

  def configure(self, addon_name, local_behavior_packs_path=None, local_resource_packs_path=None, local_skin_packs_path=None):
    """Configure the client with addon name and local paths"""
    self.addon_name = addon_name
    self.local_behavior_packs_path = local_behavior_packs_path
    self.local_resource_packs_path = local_resource_packs_path
    self.local_skin_packs_path = local_skin_packs_path
    self.is_configured = True

  def push(self, src_path: str, dest_path: str):
    self.house_arrest.push(src_path, dest_path)
  
  def push_behavior_pack(self, src_path: str, dest_path: str):
    dest_path = path.join(DEV_BEHAVIOR_PACKS_PATH, dest_path)
    self.house_arrest.makedirs(path.dirname(dest_path))
    self.push(src_path, dest_path)
  
  def push_resource_pack(self, src_path: str, dest_path: str):
    dest_path = path.join(DEV_RESOURCE_PACKS_PATH, dest_path)
    self.house_arrest.makedirs(path.dirname(dest_path))
    self.push(src_path, dest_path)

  def push_skin_pack(self, src_path: str, dest_path: str):
    dest_path = path.join(DEV_SKIN_PACKS_PATH, dest_path)
    self.house_arrest.makedirs(path.dirname(dest_path))
    self.push(src_path, dest_path)

  def list_behavior_packs(self):
    return self.house_arrest.listdir(filename=DEV_BEHAVIOR_PACKS_PATH)

  def list_resource_packs(self):
    return self.house_arrest.listdir(filename=DEV_RESOURCE_PACKS_PATH)

def send_response(success, message, data=None, request_id=None):
    """Send a JSON response to stdout"""
    response = {
        "success": success,
        "message": message,
        "data": data
    }
    if request_id is not None:
        response["requestId"] = request_id
    print(json.dumps(response), flush=True)

def handle_command(client, command):
    """Handle a command received from stdin"""
    try:
        cmd_type = command.get("type")
        request_id = command.get("requestId")
        
        # Configuration command
        if cmd_type == "configure":
            addon_name = command.get("addon_name")
            local_behavior_packs_path = command.get("local_behavior_packs_path")
            local_resource_packs_path = command.get("local_resource_packs_path")
            local_skin_packs_path = command.get("local_skin_packs_path")
            
            if not addon_name:
                send_response(False, "Missing required parameter: addon_name", request_id=request_id)
                return
                
            client.configure(
                addon_name=addon_name,
                local_behavior_packs_path=local_behavior_packs_path,
                local_resource_packs_path=local_resource_packs_path,
                local_skin_packs_path=local_skin_packs_path
            )
            send_response(True, f"Successfully configured daemon with addon name: {addon_name}", request_id=request_id)
            return
            
        # Check if client is configured for other commands
        if not client.is_configured:
            send_response(False, "Daemon not configured. Please send a 'configure' command first.", request_id=request_id)
            return
            
        # Push commands with relative paths
        if cmd_type == "push_behavior_pack":
            if not client.local_behavior_packs_path:
                send_response(False, "Local behavior packs path not configured", request_id=request_id)
                return
                
            relative_path = command.get("path")
            if not relative_path:
                send_response(False, "Missing required parameter: path", request_id=request_id)
                return
                
            src_path = path.join(client.local_behavior_packs_path, relative_path)
            dest_path = path.join(client.addon_name, relative_path)
            client.push_behavior_pack(src_path, dest_path)
            send_response(True, f"Successfully pushed {relative_path} to behavior pack {dest_path}", request_id=request_id)
            
        elif cmd_type == "push_resource_pack":
            if not client.local_resource_packs_path:
                send_response(False, "Local resource packs path not configured", request_id=request_id)
                return
                
            relative_path = command.get("path")
            if not relative_path:
                send_response(False, "Missing required parameter: path", request_id=request_id)
                return
                
            src_path = path.join(client.local_resource_packs_path, relative_path)
            dest_path = path.join(client.addon_name, relative_path)
            client.push_resource_pack(src_path, dest_path)
            send_response(True, f"Successfully pushed {relative_path} to resource pack {dest_path}", request_id=request_id)
            
        elif cmd_type == "push_skin_pack":
            if not client.local_skin_packs_path:
                send_response(False, "Local skin packs path not configured", request_id=request_id)
                return
                
            relative_path = command.get("path")
            if not relative_path:
                send_response(False, "Missing required parameter: path", request_id=request_id)
                return
                
            src_path = path.join(client.local_skin_packs_path, relative_path)
            dest_path = path.join(client.addon_name, relative_path)
            client.push_skin_pack(src_path, dest_path)
            send_response(True, f"Successfully pushed {relative_path} to skin pack {dest_path}", request_id=request_id)
            
        elif cmd_type == "list_behavior_packs":
            packs = client.list_behavior_packs()
            send_response(True, "Successfully listed behavior packs", packs, request_id=request_id)
            
        elif cmd_type == "list_resource_packs":
            packs = client.list_resource_packs()
            send_response(True, "Successfully listed resource packs", packs, request_id=request_id)
            
        else:
            send_response(False, f"Unknown command type: {cmd_type}", request_id=request_id)
            
    except Exception as e:
        send_response(False, f"Error executing command: {str(e)}", request_id=request_id)

def main():
    try:
        client = MinepackClient()
        send_response(True, "Minepack daemon started successfully")
        
        # Main loop to read commands from stdin
        while True:
            try:
                line = sys.stdin.readline()
                if not line:
                    break
                    
                command = json.loads(line)
                handle_command(client, command)
                
            except json.JSONDecodeError:
                send_response(False, "Invalid JSON command")
            except Exception as e:
                send_response(False, f"Error processing command: {str(e)}")
                
    except Exception as e:
        send_response(False, f"Fatal error: {str(e)}")
        sys.exit(1)

if __name__ == "__main__":
    main()
