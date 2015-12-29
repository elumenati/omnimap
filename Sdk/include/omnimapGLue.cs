
using System;
using System.Runtime.InteropServices;


namespace OmnimapLibrary
{
    static class OmnimapDLL
    {
        private static class NativeMethods
        {
            [DllImport("kernel32.dll")]
            public static extern IntPtr LoadLibrary(string dllToLoad);

            [DllImport("kernel32.dll")]
            public static extern IntPtr GetProcAddress(IntPtr hModule, string procedureName);

            [DllImport("kernel32.dll")]
            public static extern bool FreeLibrary(IntPtr hModule);
        }


        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate IntPtr UnmanagedGlue_Constructor(Int32 resW, Int32 resH, IntPtr ptr1, IntPtr ptr2, //ptr1 and ptr2 are the handles for d3d9, d3d10 and d3d11
                                                         [MarshalAs(UnmanagedType.LPStr)]string strStartUpScriptFile, [MarshalAs(UnmanagedType.LPStr)]string luaResDir);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void UnmanagedGlue_Destructor(IntPtr omnimapHandle);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate Int32 UnmanagedGlue_GetChannelTotal(IntPtr omnimapHandle);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate IntPtr UnmanagedGlue_GetChannelInfo(IntPtr omnimapHandle, Int32 channelNumber,
                                                          [MarshalAs(UnmanagedType.LPArray, SizeConst = 16)] float[] viewMatrix, [MarshalAs(UnmanagedType.LPArray, SizeConst = 16)] float[] projMatrix);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void UnmanagedGlue_BeginRenderChannel(IntPtr channelHandle);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void UnmanagedGlue_EndRenderChannel(IntPtr channelHandle);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void UnmanagedGlue_PostRender(IntPtr omnimapHandle);

        private static IntPtr DllHandle = IntPtr.Zero;

        private static UnmanagedGlue_Constructor UG_Constructor = null;
        private static UnmanagedGlue_Destructor UG_Destructor = null;
        private static UnmanagedGlue_GetChannelTotal UG_GetChannelTotal = null;
        private static UnmanagedGlue_GetChannelInfo UG_GetChannelInfo = null;
        private static UnmanagedGlue_BeginRenderChannel UG_BeginRenderChannel = null;
        private static UnmanagedGlue_EndRenderChannel UG_EndRenderChannel = null;
        private static UnmanagedGlue_PostRender UG_PostRender = null;



        public static bool IsLoaded()
        {
            return (DllHandle != IntPtr.Zero);
        }


        // valid api strings: D3D9, D3D10, D3D11, OGL
        public static bool Constructor(string apiToUse, bool useDebug = false)
        {
            IntPtr funcPtr = IntPtr.Zero;
            string omnimapDllName = useDebug ? @"omnimapD.DLL" : @"omnimap.DLL";

            if (IsLoaded()) return true;

            DllHandle = NativeMethods.LoadLibrary(omnimapDllName);
            if (!IsLoaded())
            {
                string saveCurrentDir = Environment.CurrentDirectory;
                Environment.CurrentDirectory = saveCurrentDir + "\\32bits";
                DllHandle = NativeMethods.LoadLibrary(omnimapDllName);
                Environment.CurrentDirectory = saveCurrentDir;
            }
            if (!IsLoaded())
            {
                string saveCurrentDir = Environment.CurrentDirectory;
                Environment.CurrentDirectory = saveCurrentDir + "\\64bits";
                DllHandle = NativeMethods.LoadLibrary(omnimapDllName);
                Environment.CurrentDirectory = saveCurrentDir;
            }

            if (!IsLoaded()) return true;

            funcPtr = NativeMethods.GetProcAddress(DllHandle, "UnmanagedGlue_OmnimapConstructor" + apiToUse);
            if (funcPtr == IntPtr.Zero) return false;
            UG_Constructor = (UnmanagedGlue_Constructor)Marshal.GetDelegateForFunctionPointer(funcPtr, typeof(UnmanagedGlue_Constructor));
            funcPtr = NativeMethods.GetProcAddress(DllHandle, "UnmanagedGlue_OmnimapDestructor" + apiToUse);
            if (funcPtr == IntPtr.Zero) return false;
            UG_Destructor = (UnmanagedGlue_Destructor)Marshal.GetDelegateForFunctionPointer(funcPtr, typeof(UnmanagedGlue_Destructor));
            funcPtr = NativeMethods.GetProcAddress(DllHandle, "UnmanagedGlue_OmnimapGetChannelTotal" + apiToUse);
            if (funcPtr == IntPtr.Zero) return false;
            UG_GetChannelTotal = (UnmanagedGlue_GetChannelTotal)Marshal.GetDelegateForFunctionPointer(funcPtr, typeof(UnmanagedGlue_GetChannelTotal));
            funcPtr = NativeMethods.GetProcAddress(DllHandle, "UnmanagedGlue_OmnimapGetChannelInfo" + apiToUse);
            if (funcPtr == IntPtr.Zero) return false;
            UG_GetChannelInfo = (UnmanagedGlue_GetChannelInfo)Marshal.GetDelegateForFunctionPointer(funcPtr, typeof(UnmanagedGlue_GetChannelInfo));
            funcPtr = NativeMethods.GetProcAddress(DllHandle, "UnmanagedGlue_OmnimapBeginRenderChannel" + apiToUse);
            if (funcPtr == IntPtr.Zero) return false;
            UG_BeginRenderChannel = (UnmanagedGlue_BeginRenderChannel)Marshal.GetDelegateForFunctionPointer(funcPtr, typeof(UnmanagedGlue_BeginRenderChannel));
            funcPtr = NativeMethods.GetProcAddress(DllHandle, "UnmanagedGlue_OmnimapEndRenderChannel" + apiToUse);
            if (funcPtr == IntPtr.Zero) return false;
            UG_EndRenderChannel = (UnmanagedGlue_EndRenderChannel)Marshal.GetDelegateForFunctionPointer(funcPtr, typeof(UnmanagedGlue_EndRenderChannel));
            funcPtr = NativeMethods.GetProcAddress(DllHandle, "UnmanagedGlue_OmnimapPostRender" + apiToUse);
            if (funcPtr == IntPtr.Zero) return false;
            UG_PostRender = (UnmanagedGlue_PostRender)Marshal.GetDelegateForFunctionPointer(funcPtr, typeof(UnmanagedGlue_PostRender));

            return true;
        }


        public static void Destructor()
        {
            if (!IsLoaded()) return;

            API.Destructor();

            NativeMethods.FreeLibrary(DllHandle);
            DllHandle = IntPtr.Zero;
        }







        public static class API
        {
            private static IntPtr handle = IntPtr.Zero;
            private static IntPtr[] channelHandle = new IntPtr[6];


            public delegate void LoadMatrices(int i, float[] viewMatrix, float[] projMatrix);


            public static bool IsLoaded()
            {
                return (handle != IntPtr.Zero);
            }


            private static bool Constructor(Int32 resW, Int32 resH, IntPtr ptr1, IntPtr ptr2, string strStartUpScriptFile, string luaResDir, LoadMatrices func)
            {
                if (!OmnimapDLL.IsLoaded()) return true;

                Destructor();

                handle = UG_Constructor(resW, resH, ptr1, ptr2, strStartUpScriptFile, luaResDir);
                if (IsLoaded())
                {
                    int numChannels = GetTotalChannels();
                    float[] localViewMatrix = new float[16];
                    float[] localProjMatrix = new float[16];
                    for (int i = 0; i < numChannels; i++)
                    {
                        channelHandle[i] = UG_GetChannelInfo(handle, i, localViewMatrix, localProjMatrix);
                        func(i, localViewMatrix, localProjMatrix);
                    }

                    return true;
                }

                return false;
            }


            public static bool ConstructorOGL(Int32 resW, Int32 resH, string strStartUpScriptFile, string luaResDir, LoadMatrices func)
            {
                return Constructor(resW, resH, IntPtr.Zero, IntPtr.Zero, strStartUpScriptFile, luaResDir, func);
            }


            public static bool ConstructorD3D9(Int32 resW, Int32 resH, IntPtr d3dDevice, string strStartUpScriptFile, string luaResDir, LoadMatrices func)
            {
                return Constructor(resW, resH, d3dDevice, IntPtr.Zero, strStartUpScriptFile, luaResDir, func);
            }


            public static bool ConstructorD3D10(Int32 resW, Int32 resH, IntPtr d3dDevice, string strStartUpScriptFile, string luaResDir, LoadMatrices func)
            {
                return Constructor(resW, resH, d3dDevice, IntPtr.Zero, strStartUpScriptFile, luaResDir, func);
            }


            public static bool ConstructorD3D11(Int32 resW, Int32 resH, IntPtr d3dDevice, IntPtr d3dDeviceContext, string strStartUpScriptFile, string luaResDir, LoadMatrices func)
            {
                return Constructor(resW, resH, d3dDevice, d3dDeviceContext, strStartUpScriptFile, luaResDir, func);
            }


            public static void Destructor()
            {
                if (!OmnimapDLL.IsLoaded()) return;
                if (!IsLoaded()) return;

                UG_Destructor(handle);
                handle = IntPtr.Zero;
            }


            public static int GetTotalChannels()
            {
                return UG_GetChannelTotal(handle);
            }


            public static void BeginRenderChannel(int channel)
            {
                UG_BeginRenderChannel(channelHandle[channel]);
            }


            public static void EndRenderChannel(int channel)
            {
                UG_EndRenderChannel(channelHandle[channel]);
            }


            public static void PostRender()
            {
                UG_PostRender(handle);
            }
        }
    }
}
