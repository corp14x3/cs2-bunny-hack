import pymem , time , ctypes , win32api , win32con

VK_XBUTTON1 = 0x05
class MOUSEINPUT(ctypes.Structure):
    _fields_ = [("dx", ctypes.c_long),("dy", ctypes.c_long),("mouseData", ctypes.c_ulong),("dwFlags", ctypes.c_ulong),("time", ctypes.c_ulong),("dwExtraInfo", ctypes.POINTER(ctypes.c_ulong)),]

class INPUT(ctypes.Structure):
    class _INPUT(ctypes.Union):
        _fields_ = [("mi", MOUSEINPUT)]
    _anonymous_ = ("u",)
    _fields_ = [("type", ctypes.c_ulong), ("u", _INPUT)]

def is_button4_pressed():
    return win32api.GetAsyncKeyState(win32con.VK_XBUTTON1) & 0x8000 != 0


cs2 = pymem.Pymem("cs2.exe")
client = pymem.process.module_from_name(cs2.process_handle, "client.dll").lpBaseOfDll

plus = 65537
minus = 256

m_fFlags = 0x3EC;
dwLocalPlayerPawn = cs2.read_longlong(client + 0x18530D0)

while 1:
    jump = client + 0x184BE00
    characterStatus = cs2.read_int(dwLocalPlayerPawn + m_fFlags)
    if is_button4_pressed():
        if characterStatus in [65667, 65665]:
            cs2.write_int(jump,plus)
            time.sleep(0.01)
            cs2.write_int(jump,minus)
            time.sleep(0.01)