#define DISABLE_PERSISTENCY

// ===== core/BaseModuleInclude.c =====
// ===== core/ModuleManager.c =====
#define COM_MODULES_OLDLOADING

// ===== core/Module.c =====
/*
	Extend this class for separate objects
	Only override mouse and key functions for extra utility
*/
class Module
{
	protected bool m_Enabled;
	protected bool m_PreventInput;
	protected ref set< ref KeyMouseBinding > m_KeyBindings;
	
	void Module()
	{
		m_Enabled = true;
		m_PreventInput = false;
		m_KeyBindings = new set< ref KeyMouseBinding >;
	}
	
	void ~Module()
	{
	}
	
	void Init()
	{
		RegisterKeyMouseBindings();
	}

    void Toggle()
    {
        m_Enabled = !m_Enabled;
    }

	void PreventInput(bool prevent)
	{
		m_PreventInput = prevent;
	}

    string GetModuleName()
    {
        return ClassName();
    }

    typename GetModuleType()
    {
        return GetModuleName().ToType();
    }

    bool IsEnabled()
    {
        return m_Enabled;
    }

	bool IsPreventingInput()
	{
		return m_PreventInput;
	}

	void RegisterKeyMouseBindings() 
	{
	}
	
	void RegisterKeyMouseBinding( KeyMouseBinding binding ) 
	{
        m_KeyBindings.Insert( binding );

        UAInput input = GetUApi().RegisterInput( binding.GetUAInputName(), binding.GetDescription(), "core" );

        if ( input == NULL ) return;

        input.AddAlternative();

        auto bindings = binding.GetBindings();
        for ( int i = 0; i < bindings.Count(); i++ )
        {
            input.BindCombo( bindings[i] );
        }
	}
	
	set< ref KeyMouseBinding > GetBindings()
	{
		return m_KeyBindings;
	}

	// Override functions 
	
	void onUpdate( float timeslice ) 
	{
	}
	
	/* Mouse Functions */ 
	
	void onMouseMove() 
	{
	}
	
	void onMouseDragY ( int state ) 
	{
	}
	
	void onMouseDragX ( int state )
	{
	}
	
	void onMouseWheelUp( int state ) 
	{
	}
	
	void onMouseWheelDown( int state ) 
	{
	}
	
	void onMouseButtonPress( int button )
	{
	}
	
	void onMouseButtonRelease( int button )
	{
	}

	/* Key Functions */
	
	void onKeyPress( int key ) 
	{
	}
	
	void onKeyRelease( int key )
	{
	}
	
	/* Mission Functions */
	
	void onMissionStart()
	{
	}
	
	void onMissionFinish()
	{
	}
	
	void onMissionLoaded()
	{
	}
}

// ===== core/KeyMouseBinding.c =====
class KeyMouseBinding
{
    protected typename m_Object;
    protected ref array< string > m_KeyBinds;

    protected string m_CallbackFunction;
    protected string m_UAInputName;
    protected string m_Description;

    protected bool m_CanBeUsedInMenu;

    protected int m_ActionType;
    
    void KeyMouseBinding( typename object, string callback, string description, bool menu = false ) 
    {
        m_Object = object;
        m_KeyBinds = new array< string >;
        
        m_CallbackFunction = callback;
        m_UAInputName = "UA" + object.ToString() + callback;

        m_Description = description;
        
        m_CanBeUsedInMenu = menu;

        m_ActionType = KeyMouseActionType.PRESS;
    }
    
    bool CanBeUsedInMenu() 
    {
        return m_CanBeUsedInMenu;
    }
    
    void AddBinding( string key, int action = 1)
    {
        m_KeyBinds.Insert( key );
        m_ActionType = action;
    }
    
    array< string > GetBindings() 
    {
        return m_KeyBinds;
    }

    void SetActionType( int type ) 
    {
        m_ActionType = type;
    }

    int GetActionType()
    {
        return m_ActionType;
    }
    
    typename GetObject() 
    {
        return m_Object;
    }
    
    string GetCallBackFunction() 
    {
        return m_CallbackFunction;
    }

    string GetUAInputName()
    {
        return m_UAInputName;
    }

    string GetDescription()
    {
        return m_Description;
    }
}

#ifdef COM_MODULES_OLDLOADING
// ===== core/modules/AdminTool/module.c =====
/*
    Define used for optional compilations
*/
#define MODULE_ADMIN_TOOL

/*
    Include of all .c files that belong to this module
*/
//#include "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\Admintool\\gui\\PositionMenu.c"
// ===== core/modules/CameraTool/module.c =====
#ifndef COM_MODULES_OLDLOADING
#endif

/*
    Define used for optional compilations
*/
#define MODULE_CAMERA_TOOL

/*
    Include of all .c files that belong to this module
*/
#ifdef COM_MODULES_OLDLOADING
// ===== core/modules/CameraTool/CameraTool.c =====
class CameraTool extends Module
{
	protected Camera m_oCamera; // active static camera "staticcamera"

	protected ref array<vector> m_cKeyframes = new array<vector>;

	protected vector velocity; // canera velocity

	protected float yawVelocity;
	protected float pitchVelocity;

	protected float m_CamFOV = 1.0; // default FOV
	protected float m_TargetFOV = 1.0;
	protected float m_TargetRoll;
	protected float m_DistanceToObject;
	protected bool m_FollowTarget = false;
	protected bool m_FreezePlayer = false;
	protected bool m_OrbitalCam = false;
	protected bool m_FreezeCam = false;

	protected bool m_FreezeMouse = false;
	
	static float CAMERA_FOV = 1.0;
	static float CAMERA_TARGETFOV = 1.0;
	static float CAMERA_FOV_SPEED_MODIFIER = 5.0;
	static float CAMERA_SPEED = 5.0;
	static float CAMERA_VELDRAG = 0.9; // 0.9 - 1.0 0.9 == no smoothing
	static float CAMERA_MSENS = 0.8; // acceleration
	static float CAMERA_SMOOTH = 0.65; // drag

	static bool  CAMERA_DOF = false;
	static bool  CAMERA_AFOCUS = true;
	static float CAMERA_BLUR = 0.0; // modified via ui
	static float CAMERA_FLENGTH = 50.0; // modified via ui
	static float CAMERA_FNEAR = 50.0; // modified via ui
	static float CAMERA_FDIST = 0.0;
	static float CAMERA_DOFFSET = 0.0;

	static float CAMERA_SMOOTH_BLUR = 0.0;

	static float EXPOSURE = 0.0;
	static float CHROMABERX = 0.0; // these need to go somewhere else. not where this object is GC'd
	static float CHROMABERY = 0.0;
	static float HUESHIFT = 0.0;

	static float ROTBLUR = 0.0;
	static float MINDEPTH = 2.5;
	static float MAXDEPTH = 4.5;

	static float RADBLURX = 0.0;
	static float RADBLURY = 0.0;
	static float RADBLUROFFX = 0.0;
	static float RADBLUROFFY = 0.0;

	static float VIGNETTE = 0.0;
	static float VARGB[4] = { 0, 0, 0, 0 };

	static float CARGB[4] = { 0, 0, 0, 1 }; // color overlay

	static int VIEWDISTANCE = 1600; // move later

	protected vector m_CamOffset;
	
	protected Object m_Target;
	protected vector m_TargetPos; // Static position
	
	protected float m_CurrentSmoothBlur;

	void CameraTool()
	{
	}

	void ~CameraTool()
	{
	}
	
	override void Init() 
	{
		super.Init();
	}
	
	override void onUpdate( float timeslice )
	{
		float speed = 0.2;
		m_CurrentSmoothBlur = Math.Lerp( m_CurrentSmoothBlur, CAMERA_SMOOTH_BLUR, speed );
		PPEffects.SetBlur( m_CurrentSmoothBlur );

		UpdateCamera( timeslice );
	}
	
	override void RegisterKeyMouseBindings() 
	{
		KeyMouseBinding toggleCamera  = new KeyMouseBinding( GetModuleType(), "ToggleCamera" , "Toggle camera."  );
		KeyMouseBinding freezeCamera  = new KeyMouseBinding( GetModuleType(), "FreezeCamera" , "Freezes camera." );
		KeyMouseBinding freezePlayer  = new KeyMouseBinding( GetModuleType(), "FreezePlayer" , "Freezes player.", true);
		KeyMouseBinding followTarget  = new KeyMouseBinding( GetModuleType(), "FollowTarget" , "Follows target.", true );
		KeyMouseBinding toggleOrbit   = new KeyMouseBinding( GetModuleType(), "ToggleOrbital", "Toggle orbital mode", true );
		KeyMouseBinding targetCamera  = new KeyMouseBinding( GetModuleType(), "TargetCamera" , "Targets objects or positions", true );
		KeyMouseBinding zoomCamera    = new KeyMouseBinding( GetModuleType(), "ZoomCamera"   , "Zooms camera" );
		KeyMouseBinding incCamSpeed   = new KeyMouseBinding( GetModuleType(), "IncCamSpeed"  , "Increase camera speed" );
		KeyMouseBinding decCamSpeed   = new KeyMouseBinding( GetModuleType(), "DecCamSpeed"  , "Decrease camera speed" );

		KeyMouseBinding release       = new KeyMouseBinding( GetModuleType(), "Release"		 , "Release mouse", true);

		toggleCamera.AddBinding( "kInsert" );
		freezeCamera.AddBinding( "kBackslash" );
		freezePlayer.AddBinding( "kCapital" );
		followTarget.AddBinding( "kLBracket" );
		toggleOrbit .AddBinding( "kRBracket" );
		
		targetCamera.AddBinding( "mBMiddle" );
		
		zoomCamera.AddBinding( "mBRight", KeyMouseActionType.HOLD );
		zoomCamera.AddBinding( "kLControl", KeyMouseActionType.HOLD );

		incCamSpeed.AddBinding( "mWheelUp" );
		decCamSpeed.AddBinding( "mWheelDown" );

		release.AddBinding( "mBRight", KeyMouseActionType.RELEASE );

//		zoomCamera    .AddBinding( MouseState.WHEEL, 0 );
		
		RegisterKeyMouseBinding( toggleCamera );
		RegisterKeyMouseBinding( freezeCamera );
		RegisterKeyMouseBinding( freezePlayer );
		RegisterKeyMouseBinding( followTarget );
		RegisterKeyMouseBinding( toggleOrbit  );
		RegisterKeyMouseBinding( targetCamera );
		RegisterKeyMouseBinding( zoomCamera   );
		RegisterKeyMouseBinding( incCamSpeed  );
		RegisterKeyMouseBinding( decCamSpeed  );
		RegisterKeyMouseBinding( release );
	}

	Camera GetCamera()
	{
		return m_oCamera;
	}

	void EnableCamera( bool staticCam = false )
	{
		if ( m_oCamera )
		{
			return;
		}

		vector position = "0 0 0";

		if ( COM_GetPB() )
		{
			position = COM_GetPB().GetPosition();
			position[ 1 ] = position[ 1 ] + 2;
		}

		m_oCamera = Camera.Cast(g_Game.CreateObject( "staticcamera", position, false ));
		m_oCamera.SetActive( true );

		if ( !staticCam ) 
		{
			SetFreezePlayer( true );
		}
		
		m_DistanceToObject = 0.0;
	}

	void DisableCamera()
	{
		if ( m_oCamera )
		{
		    //Close menu to give back control to player
            if( GetGame().GetUIManager().GetMenu() && ( GetGame().GetUIManager().GetMenu().GetID() == 133742 ) )
            {
                GetGame().GetUIManager().Back();
            }

			SetFreezePlayer( false );
			SetFreezeMouse( false );

			vector position;

			if( COM_CTRL() || COM_SHIFT() ) // Extra
			{
				position = m_oCamera.GetPosition();
				position[ 1 ] = GetGame().SurfaceY( position[ 0 ], position[ 2 ] );
			}
			else
			{
				position = COM_GetCursorPos();
			}

			if ( COM_GetPB() )
			{
				COM_GetPB().SetPosition( position );
			}

			m_oCamera.SetActive( false );

			GetGame().SelectPlayer( NULL, COM_GetPB() );

			GetGame().ObjectDelete( m_oCamera );

			m_oCamera = NULL;

			m_CamFOV = 1.0;
			m_TargetFOV = 1.0;
			m_TargetRoll = 0;

			m_FollowTarget = false;
			m_OrbitalCam = false;

			m_Target = NULL;
			m_TargetPos = vector.Zero;

			PPEffects.ResetDOFOverride();
		}
	}
	
	void ToggleCamera() 
	{
		if ( m_oCamera )
		{
			DisableCamera();
		}
		else
		{
			EnableCamera();
		}
	}
	
	void TargetCamera() 
	{
		if ( m_oCamera ) 
		{
			if ( GetGame().GetUIManager().IsCursorVisible() ) 
			{
				return;
			}

			if ( m_Target || m_TargetPos != vector.Zero ) 
			{
				// Stop targetting
				m_Target = NULL;
				m_TargetPos = vector.Zero;
			
				SetFreezeMouse(false);

				return;
			}
			
			vector dir = GetGame().GetPointerDirection();
			vector from = GetGame().GetCurrentCameraPosition();
			vector to = from + ( dir * 1000 );
			
			set< Object > objects = COM_GetObjectsAt(from, to);
			
			if ( objects ) 
			{
				if ( objects.Count() > 0) 
				{
					Object object = objects.Get(0);
					if ( object.IsInherited( EntityAI ) && !object.IsBuilding() )
					{
						m_Target = object;
						return;
					} 
				}
			}
			m_TargetPos = COM_GetCursorPos();
		}
	}

	void SetTarget(Object oObject)
	{
		if (oObject)
		{
			m_Target = oObject;
			m_TargetPos = oObject.GetPosition();
		} else
		{
			m_Target = NULL;
		}
	}
	
	void UpdateCamera( float timeslice) 
	{
		if ( m_oCamera ) 
		{

			if ( m_CamFOV != m_TargetFOV ) 
			{
				m_CamFOV = Math.Lerp( m_CamFOV, m_TargetFOV, timeslice*CAMERA_FOV_SPEED_MODIFIER );
				m_oCamera.SetFOV( m_CamFOV );
			}

			vector oldOrient = m_oCamera.GetOrientation();
			if ( oldOrient[2] != m_TargetRoll ) 
			{
				//oldOrient[2] = Math.Lerp( oldOrient[2], m_TargetRoll, timeslice*CAMERA_FOV_SPEED_MODIFIER );
				//m_oCamera.SetOrientation( oldOrient );
			}

			// Camera movement
			Input input = GetGame().GetInput();

			if ( !m_FreezeCam ) 
			{

				float forward = KeyState(KeyCode.KC_W) - KeyState(KeyCode.KC_S); // -1, 0, 1
				float strafe = KeyState(KeyCode.KC_D) - KeyState(KeyCode.KC_A);
				float altitude = KeyState(KeyCode.KC_Q) - KeyState(KeyCode.KC_Z); // change to hardcode keys? these actions can be rebinded via vanilla keybind menu

				if( KeyState(KeyCode.KC_LSHIFT) ) 
				{
					forward *= 10.0;
					strafe *= 10.0;
					altitude *= 10.0;
				}
				float cam_speed = CAMERA_SPEED;
				float drag = CAMERA_VELDRAG;

				if ( CAMERA_VELDRAG == 0.9 ) 
				{
					cam_speed *= 15.0;
					drag = 0;
				}

				vector up = vector.Up;
				vector direction = m_oCamera.GetDirection();
				vector directionAside = vector.Up * direction;

				up = up * altitude * cam_speed * timeslice;
				direction = direction * forward * cam_speed * timeslice;
				directionAside = directionAside * strafe * cam_speed * timeslice;

				velocity = velocity * drag;
				velocity = ( velocity + direction + directionAside + up );

				vector oldPos = m_oCamera.GetPosition();
				vector newPos = oldPos + velocity;

				float surfaceY = GetGame().SurfaceY( newPos[0], newPos[2] ) + 0.25;
				if ( newPos[1] < surfaceY ) 
				{
					newPos[1] = surfaceY;
				}

				m_oCamera.SetPosition(newPos);
			}

			if ( !m_FreezeMouse ) 
			{
				float yawDiff = input.LocalValue("UAAimLeft") - input.LocalValue("UAAimRight");
				float pitchDiff = input.LocalValue("UAAimDown") - input.LocalValue("UAAimUp");

				yawVelocity = yawVelocity + yawDiff * CAMERA_MSENS;
				pitchVelocity = pitchVelocity + pitchDiff * CAMERA_MSENS; // 0.8

				vector newOrient = oldOrient;

				newOrient[0] = oldOrient[0] - Math.RAD2DEG * yawVelocity * timeslice;
				newOrient[1] = oldOrient[1] - Math.RAD2DEG * pitchVelocity * timeslice;

				yawVelocity *= CAMERA_SMOOTH; // drag 0.9
				pitchVelocity *= CAMERA_SMOOTH;

				if( newOrient[1] < -89 )
					newOrient[1] = -89;
				if( newOrient[1] > 89 )
					newOrient[1] = 89;

				m_oCamera.SetOrientation( newOrient );
			}

			// Camera targetting
			float dist = 0.0;
			vector from = GetGame().GetCurrentCameraPosition();
	
			if ( m_Target ) 
			{
				vector targetPos;
				
				if ( m_Target.IsInherited( SurvivorBase ) ) 
				{
					targetPos = GetTargetCenter();
				}
				else 
				{
					vector pos = m_Target.GetPosition();
					pos[1] = GetGame().SurfaceY(pos[0], pos[2]);
					
					vector clippingInfo;
					vector objectBBOX;
					
					m_Target.GetCollisionBox(objectBBOX);
					
					pos[1] = (pos[1] - objectBBOX[1] + clippingInfo[1] - objectBBOX[1]) + 1.5;
					
					targetPos = pos;
				}
				
				if ( m_OrbitalCam ) 
				{
					m_oCamera.LookAt( targetPos );
				}
				
				dist = vector.Distance( from, targetPos );
				
				if ( m_FollowTarget ) 
				{
					if ( m_DistanceToObject == 0.0 )
					{
						m_DistanceToObject = vector.Distance(GetTargetCenter(), m_oCamera.GetPosition());
						m_CamOffset = vector.Direction( GetTargetCenter() , m_oCamera.GetPosition() );
						m_CamOffset.Normalize();
					}
					
					if ( m_OrbitalCam ) 
					{
						direction = vector.Direction( GetTargetCenter() , m_oCamera.GetPosition() );
						direction.Normalize();
						newPos = GetTargetCenter() + ( direction * m_DistanceToObject );
					} 
					else 
					{
						newPos = GetTargetCenter() + ( m_CamOffset * m_DistanceToObject );
					}
					
					m_oCamera.SetPosition( newPos );
					dist = m_DistanceToObject;
				}
			}
			else if ( m_TargetPos != vector.Zero ) 
			{
				// m_oCamera.LookAt( m_TargetPos ); // auto orbital
				vector lookDir = m_TargetPos - m_oCamera.GetPosition();
				float roll = m_oCamera.GetOrientation()[2];
				m_oCamera.SetDirection( lookDir );

				vector newRoll = m_oCamera.GetOrientation();
				newRoll[2] = roll;
				m_oCamera.SetOrientation(newRoll);
				dist = vector.Distance( from, m_TargetPos );
			}
			
			if ( CAMERA_DOF ) // DOF enabled
			{
				if ( CAMERA_AFOCUS && !m_Target ) //auto focus
				{
					vector to = from + (GetGame().GetCurrentCameraDirection() * 9999);
					vector contact_pos;
					
					DayZPhysics.RaycastRV( from, to, contact_pos, NULL, NULL, NULL , NULL, NULL, false, false, ObjIntersectIFire);
					dist = vector.Distance( from, contact_pos );
				}
				if ( dist > 0 ) CAMERA_FDIST = dist;
				
				m_oCamera.SetFocus(CAMERA_FDIST, CAMERA_BLUR);
				//m_oCamera.SetNearPlane(CAMERA_FNEAR);
				// PPEffects.OverrideDOF(true, CAMERA_FDIST, CAMERA_FLENGTH, CAMERA_FNEAR, CAMERA_BLUR, CAMERA_DOFFSET); broken :(
			}
		}
	}
	
	void IncCamSpeed() 
	{
		if ( m_oCamera ) 
		{
			if ( GetGame().GetUIManager().IsCursorVisible() ) 
			{
				return;
			}
			CAMERA_SPEED *= 1.2;
		}
	}
	
	void DecCamSpeed() 
	{
		if ( m_oCamera ) 
		{
			if ( GetGame().GetUIManager().IsCursorVisible() ) 
			{
				return;
			}
			
			CAMERA_SPEED *= 0.8;
			if ( CAMERA_SPEED < 0.001 ) 
			{
				CAMERA_SPEED = 0.001;
			}
		}
	}

	void CameraSpeed() 
	{
		if ( m_oCamera ) 
		{
			if ( GetGame().GetUIManager().IsCursorVisible() ) 
			{
				return;
			}
			int i = GetMouseState( MouseState.WHEEL );

			if ( COM_ALT() ) 
			{
				vector ori = m_oCamera.GetOrientation();
				ori[2] = ori[2] - i*5;
				m_oCamera.SetOrientation( ori );
				//m_TargetRoll = ori[2] - i*5; // redo this
				//COM_Message(m_TargetRoll.ToString());
			}
			else 
			{
				float value = 1.2;
				if ( i < 0 ) 
				{
					value = 0.8;
				}

				CAMERA_SPEED *= value;
				if ( CAMERA_SPEED < 0.001 ) 
				{
					CAMERA_SPEED = 0.001;
				}
			}
		}
	}

	void Release() 
	{
		if ( m_oCamera ) 
		{
			if ( !m_OrbitalCam ) 
			{
				SetFreezeMouse( false );
			}
		}
	}

	void ZoomCamera() 
	{
		if ( m_oCamera ) 
		{
			if ( GetGame().GetUIManager().IsCursorVisible() ) 
			{
				return;
			}

			int i = GetMouseState(MouseState.Y);
		
			if ( i != 0 ) 
			{
				SetFreezeMouse(true);
				m_TargetFOV+=i*0.000006; // zoom speed
				
				if ( m_TargetFOV < 0.01 ) 
				{
					m_TargetFOV = 0.01;
				}
				//m_oCamera.SetFOV(m_CamFOV);
			}
		}
		
		/*
		if ( m_oCamera ) 
		{
			if ( GetGame().GetUIManager().IsCursorVisible() ) 
			{
				return;
			}

			int i = GetMouseState( MouseState.WHEEL );

			ObjectEditor objEditor = COM_GetModuleManager().GetModule( ObjectEditor );

			if ( objEditor.m_SelectedObject ) 
			{	
				return;
			}

			if ( COM_CTRL() ) 
			{
				vector ori = m_oCamera.GetOrientation();
				m_TargetRoll = ori[2] - Math.RAD2DEG * i*0.09;
			}
			else 
			{
				m_TargetFOV-=i*0.09; // invert
				if ( m_TargetFOV < 0.01 ) 
				{
					m_TargetFOV = 0.01;
				}
			}
			//m_oCamera.SetFOV(m_CamFOV);	
		}
		*/
	}
	
	void FreezeCamera() 
	{
		if ( m_oCamera ) 
		{
			SetFreezeCam(!m_FreezeCam);
		}		
	}
	
	void FollowTarget()
	{
		if ( m_oCamera ) 
		{
			if ( m_Target || m_TargetPos != vector.Zero ) 
			{
				m_FollowTarget = !m_FollowTarget;
				SetFreezeMouse( m_FollowTarget );
				SetFreezeCam( m_FollowTarget );
				
				if ( !m_FollowTarget ) 
				{
					m_DistanceToObject = 0.0;
					m_CamOffset = vector.Zero;
				}
			}
		}		
	}
	
	void FreezePlayer()
	{
		if ( m_oCamera )
		{
			SetFreezePlayer( !m_FreezePlayer );
		}	
	}
	
	void ToggleOrbital() 
	{
		if ( m_oCamera )
		{
			if ( m_Target || m_TargetPos != vector.Zero ) 
			{
				m_OrbitalCam = !m_OrbitalCam;
				SetFreezeMouse(m_OrbitalCam);
				if (! m_OrbitalCam ) 
				{
					SetFreezeCam(false);
				}
			}
		}		
	}
	
	override void onKeyRelease( int key ) // Fix
	{
		switch( key ) 
		{			
			case KeyCode.KC_W:
			case KeyCode.KC_A:
			case KeyCode.KC_S:
			case KeyCode.KC_D:
			case KeyCode.KC_Q:
			case KeyCode.KC_Z:
			{
				if ( m_oCamera )
				{
					if ( m_Target ) 
					{
						m_CamOffset = vector.Direction( GetTargetCenter() , m_oCamera.GetPosition() );
						m_CamOffset.Normalize();
						break;
					}
				}
			}
		}
	}
	
	override void onMouseButtonRelease( int button ) 
	{
		if ( m_oCamera ) 
		{
			if ( button == MouseState.RIGHT ) 
			{
				if ( !m_OrbitalCam ) 
				{
					SetFreezeMouse( false );
				}
			}
		}
	}
	
	override void onMouseButtonPress( int button ) 
	{
	}
	
	bool IsUsingCamera() 
	{
		return m_oCamera != NULL;
	}
	
	vector GetTargetCenter() 
	{
		vector targetPosition;
		
		if ( m_Target.IsInherited( SurvivorBase )) 
		{
			targetPosition = m_Target.GetPosition();
			targetPosition[1] = targetPosition[1] + 1.5;
		}
		else 
		{
			targetPosition = m_Target.GetPosition();
			targetPosition[1] = GetGame().SurfaceY(targetPosition[0], targetPosition[2]);
			
			vector clippingInfo;
			vector objectBBOX;
			
			m_Target.GetCollisionBox(objectBBOX);
			
			targetPosition[1] = (targetPosition[1] - objectBBOX[1] + clippingInfo[1] - objectBBOX[1]) + 1.5;
		}
				
		return targetPosition;
	}

	Object GetTargetObject() 
	{
		return m_Target;
	}

	vector GetTargetPos() 
	{
		return m_TargetPos;
	}
	
	void SetFreezePlayer( bool freeze ) 
	{
		m_FreezePlayer = freeze;
		COM_GetPB().GetInputController().OverrideMovementSpeed( freeze, 0 );
		COM_GetPB().GetInputController().OverrideAimChangeX( freeze, 0 );
        COM_GetPB().GetInputController().OverrideAimChangeY( freeze, 0 );
	}
	
	void SetFreezeCam( bool freeze ) 
	{
		m_FreezeCam = freeze;
	}
	
	void SetFreezeMouse( bool freeze ) 
	{
		m_FreezeMouse = freeze;
	}
};
// ===== core/modules/CameraTool/gui/CameraSettings.c =====
class CameraSettings extends PopupMenu 
{
	
	private SliderWidget m_SldCamBlur;
	private TextWidget m_TxtCamBlur;
	
	private SliderWidget m_SldCamDist;
	private TextWidget m_TxtCamDist;
	
	private SliderWidget m_SldCamFLen;
	private TextWidget m_TxtCamFlen;
	
	private SliderWidget m_SldCamFnear;
	private TextWidget m_TxtCamFnear;

	private SliderWidget m_SldCamExp;
	private TextWidget m_TxtCamExp;

	private ButtonWidget m_btn_rot;
	private ButtonWidget m_btn_phi

	static Widget CAMERA_ROT;
	static Widget CAMERA_PHI;

	private SliderWidget m_SldChromX;
	private SliderWidget m_SldChromY;

	private TextWidget m_TxtChromX;
	private TextWidget m_TxtChromY;

	private ref WidgetStore widgetStore;

	void CameraSettings()
	{
	}	
	
	void ~CameraSettings()
	{
	}
	
	override void Init()
    {
    	widgetStore = new WidgetStore( layoutRoot );

		m_SldCamBlur = SliderWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_blur") );
		m_TxtCamBlur = TextWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_blur_value") );
	
		m_SldCamDist = SliderWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_dist") );
		m_TxtCamDist = TextWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_dist_value") );
		
		m_SldCamFLen = SliderWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_flen" ) );
		m_TxtCamFlen = TextWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_flen_value") );
		
		m_SldCamFnear = SliderWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_fnear") );
		m_TxtCamFnear = TextWidget.Cast( layoutRoot.FindAnyWidget("camera_slider_fnear_value") );
		
		m_SldCamExp = SliderWidget.Cast(layoutRoot.FindAnyWidget("camera_slider_exp"));
		m_TxtCamExp = TextWidget.Cast(layoutRoot.FindAnyWidget("camera_slider_exp_value"));

		m_btn_rot = ButtonWidget.Cast( layoutRoot.FindAnyWidget("camera_btn_rot"));
		m_btn_phi = ButtonWidget.Cast( layoutRoot.FindAnyWidget("camera_btn_phi"));

		m_TxtChromX = TextWidget.Cast(layoutRoot.FindAnyWidget("camera_slider_chrom_value_x"));
		m_TxtChromY = TextWidget.Cast(layoutRoot.FindAnyWidget("camera_slider_chrom_value_y"));

		CAMERA_ROT = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\CameraTool\\gui\\layouts\\CameraROT.layout" );
		CAMERA_PHI = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\CameraTool\\gui\\layouts\\CameraPHI.layout" );
	}
	
	
    override void OnShow()
    {
        super.OnShow();

        GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert( this.Update );

        UpdateEditBox();
    }

    override void OnHide()
    {
        super.OnHide();

    }
	
	override void Update()
	{
		UpdateSliders();
	}

	void UpdateEditBox() 
	{
		widgetStore.GetEditBoxWidget("camera_input_chrom_x").SetText( CameraTool.CHROMABERX.ToString());
		widgetStore.GetEditBoxWidget("camera_input_chrom_y").SetText( CameraTool.CHROMABERY.ToString());
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		SetFocus( layoutRoot );
		return false;
	}

	override bool OnClick(Widget w, int x, int y, int button)
	{
		if ( w.GetName() == "close_button" ) 
		{
			GetLayoutRoot().Show( false );
			OnHide();
		}

		if ( w.GetName() == "camera_toggle" ) 
		{
			CameraTool cmt = CameraTool.Cast(COM_GetModuleManager().GetModule( CameraTool ));
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).Call( cmt.ToggleCamera ); // Fix crash
		}

		Widget effectsFrame = widgetStore.GetWidget( "camera_effects_frame" );
		Widget settingsFrame = widgetStore.GetWidget( "camera_settings_frame" );

		if ( w.GetName() == "camera_slider_tab_effects" ) 
		{
			if ( !effectsFrame.IsVisible() ) 
			{
				effectsFrame.Show(!effectsFrame.IsVisible());
				settingsFrame.Show(!effectsFrame.IsVisible());
			}
		}
		if ( w.GetName() == "camera_slider_tab_settings" ) 
		{
			if ( !settingsFrame.IsVisible() ) 
			{
				settingsFrame.Show(!settingsFrame.IsVisible());
				effectsFrame.Show(!settingsFrame.IsVisible());
			}
		}

		if ( w == m_btn_rot ) 
		{
			CAMERA_ROT.Show( !CAMERA_ROT.IsVisible() );
		}

		if ( w == m_btn_phi )
		{
			CAMERA_PHI.Show( !CAMERA_PHI.IsVisible() );
		}

		if ( w.GetName() == "camera_fov_speed_btn_inc" ) 
		{
			CameraTool.CAMERA_FOV_SPEED_MODIFIER += 1;
		}

		if ( w.GetName() == "camera_fov_speed_btn_dec" ) 
		{
			CameraTool.CAMERA_FOV_SPEED_MODIFIER -= 1;
			if ( CameraTool.CAMERA_FOV_SPEED_MODIFIER < 0 ) CameraTool.CAMERA_FOV_SPEED_MODIFIER = 0;
		}

		if ( w.GetName() == "camera_smooth_btn_inc" )
		{
			CameraTool.CAMERA_SMOOTH += 0.025;
			CameraTool.CAMERA_SMOOTH = Math.Clamp(CameraTool.CAMERA_SMOOTH, 0.0, 1.0); // ugh
		}

		if ( w.GetName() == "camera_smooth_btn_dec" )
		{
			CameraTool.CAMERA_SMOOTH -= 0.025;
			CameraTool.CAMERA_SMOOTH = Math.Clamp(CameraTool.CAMERA_SMOOTH, 0.0, 1.0);
		}

		if ( w.GetName() == "camera_msens_btn_inc" ) 
		{
			CameraTool.CAMERA_MSENS += 0.05;
			CameraTool.CAMERA_MSENS = Math.Clamp(CameraTool.CAMERA_MSENS, 0.0, 1.5);
		}

		if ( w.GetName() == "camera_msens_btn_dec" ) 
		{
			CameraTool.CAMERA_MSENS -= 0.05;
			CameraTool.CAMERA_MSENS = Math.Clamp(CameraTool.CAMERA_MSENS, 0.0, 1.5);
		}
		if ( w.GetName() == "camera_msmooth_btn_inc" ) 
		{
			CameraTool.CAMERA_VELDRAG += 0.01; // percent
			CameraTool.CAMERA_VELDRAG = Math.Clamp(CameraTool.CAMERA_VELDRAG, 0.9, 1.0);
		}
		if ( w.GetName() == "camera_msmooth_btn_dec" ) 
		{
			CameraTool.CAMERA_VELDRAG -= 0.01;
			CameraTool.CAMERA_VELDRAG = Math.Clamp(CameraTool.CAMERA_VELDRAG, 0.9, 1.0); // 10 clicks
		}

		return false;
	}

	override bool OnChange( Widget w, int x, int y, bool finished )
	{
		Material chromAber = GetGame().GetWorld().GetMaterial("graphics/materials/postprocess/chromaber");
		Material matColors = GetGame().GetWorld().GetMaterial("graphics/materials/postprocess/glow");
		Material rotBlur = GetGame().GetWorld().GetMaterial("graphics/materials/postprocess/rotblur");
		Material radBlur = GetGame().GetWorld().GetMaterial("graphics/materials/postprocess/radialblur");

		SliderWidget changeSlider = widgetStore.GetSliderWidget( w.GetName() );
		EditBoxWidget editBox = widgetStore.GetEditBoxWidget( w.GetName() );

		if ( editBox ) 
		{
			string text = editBox.GetText();
			float value = text.ToFloat();

			if ( editBox.GetName() == "camera_input_chrom_x" ) 
			{
				CameraTool.CHROMABERX = value;
				chromAber.SetParam( "PowerX", CameraTool.CHROMABERX );
			}

			if ( editBox.GetName() == "camera_input_chrom_y" ) 
			{
				CameraTool.CHROMABERY = value;
				chromAber.SetParam( "PowerY", CameraTool.CHROMABERY );
			}
			return false;
		}
		/*
		switch ( changeSlider.GetName() ) 
		{
			case 
		}
		*/

		if ( w == m_SldCamBlur ) 
		{
			CameraTool.CAMERA_BLUR = m_SldCamBlur.GetCurrent() * 0.01; // percent
			
			if ( m_SldCamBlur.GetCurrent() == 0 ) 
			{
				CameraTool.CAMERA_DOF = false;
				PPEffects.ResetDOFOverride();
			} 
			else 
			{
				CameraTool.CAMERA_DOF = true;
			}
		} 
		else if ( w == m_SldCamDist ) 
		{
			if ( m_SldCamDist.GetCurrent() == 0 ) 
			{
				CameraTool.CAMERA_AFOCUS = true;
			} 
			else 
			{
				CameraTool.CAMERA_AFOCUS = false;
				CameraTool.CAMERA_FDIST = m_SldCamDist.GetCurrent() * 15;
			}
		}
		else if ( w == m_SldCamFLen ) 
		{
			CameraTool.CAMERA_FLENGTH = (m_SldCamFLen.GetCurrent() * 2.0);
		}
		else if ( w == m_SldCamFnear ) 
		{
			CameraTool.CAMERA_FNEAR = (m_SldCamFnear.GetCurrent() * 2.0);
		}
		else if ( w == m_SldCamExp ) 
		{
			CameraTool.EXPOSURE = (m_SldCamExp.GetCurrent() * 0.1) - 5.0;
			GetGame().SetEVUser( CameraTool.EXPOSURE );
			//PPEffects.SetBlur( exposure );
			
			//float color[4];
			//matColors.SetParam("LensDistort", CameraTool.EXPOSURE );
			//GetPlayer().MessageStatus( CameraTool.EXPOSURE.ToString() );
			//matColors.SetParam("PowerX", exposure);
			// matColors.SetParam("PowerY", exposure);
		}
		else if ( changeSlider.GetName() == "camera_slider_hue" ) 
		{
			CameraTool.HUESHIFT = (changeSlider.GetCurrent() * 0.1) - 5.0;
			matColors.SetParam( "Saturation", CameraTool.HUESHIFT );
		}
		else if ( changeSlider.GetName() == "camera_slider_rotblur_power" ) 
		{
			CameraTool.ROTBLUR = 0.5 * (changeSlider.GetCurrent() * 0.01);
			rotBlur.SetParam( "Power", CameraTool.ROTBLUR );
		}
		else if ( changeSlider.GetName() == "camera_slider_rotblur_mindepth" ) 
		{
			CameraTool.MINDEPTH = 2.5 * (changeSlider.GetCurrent() * 0.01);
			rotBlur.SetParam( "MinDepth", CameraTool.MINDEPTH );
		}
		else if ( changeSlider.GetName() == "camera_slider_rotblur_maxdepth" ) 
		{
			CameraTool.MAXDEPTH = (changeSlider.GetCurrent() * 0.1);
			rotBlur.SetParam( "MaxDepth", CameraTool.MAXDEPTH );
		}
		else if ( changeSlider.GetName() == "camera_slider_radblur_x" ) 
		{
			CameraTool.RADBLURX = (changeSlider.GetCurrent() * 0.01);
			radBlur.SetParam( "PowerX", CameraTool.RADBLURX );
		}
		else if ( changeSlider.GetName() == "camera_slider_radblur_y" ) 
		{
			CameraTool.RADBLURY = (changeSlider.GetCurrent() * 0.01);
			radBlur.SetParam( "PowerY", CameraTool.RADBLURY );
		}
		else if ( changeSlider.GetName() == "camera_slider_radblur_offsetx" ) 
		{
			CameraTool.RADBLUROFFX = (changeSlider.GetCurrent() * 0.01);
			radBlur.SetParam( "OffsetX", CameraTool.RADBLUROFFX );
		}
		else if ( changeSlider.GetName() == "camera_slider_radblur_offsety" ) 
		{
			CameraTool.RADBLUROFFY = (changeSlider.GetCurrent() * 0.01);
			radBlur.SetParam( "OffsetY", CameraTool.RADBLUROFFY );
		}
		else if ( changeSlider.GetName() == "camera_slider_vign" ) 
		{
			CameraTool.VIGNETTE = (changeSlider.GetCurrent() * 0.02);
			matColors.SetParam( "Vignette", CameraTool.VIGNETTE );
		}
		else if ( changeSlider.GetName() == "camera_slider_vign_r" ) 
		{
			CameraTool.VARGB[0] = ((changeSlider.GetCurrent() * 0.1) - 5.0);
			matColors.SetParam("VignetteColor", CameraTool.VARGB );
		}
		else if ( changeSlider.GetName() == "camera_slider_vign_g" ) 
		{
			CameraTool.VARGB[1] = ((changeSlider.GetCurrent() * 0.1) - 5.0);
			matColors.SetParam("VignetteColor", CameraTool.VARGB );
		}
		else if ( changeSlider.GetName() == "camera_slider_vign_b" ) 
		{
			CameraTool.VARGB[2] = ((changeSlider.GetCurrent() * 0.1) - 5.0);
			matColors.SetParam("VignetteColor", CameraTool.VARGB );
		}
		else if ( changeSlider.GetName() == "camera_slider_color_r" ) 
		{
			CameraTool.CARGB[0] = ((changeSlider.GetCurrent() * 0.1) - 5.0);
			matColors.SetParam("OverlayColor", CameraTool.CARGB );
		}
		else if ( changeSlider.GetName() == "camera_slider_color_g" ) 
		{
			CameraTool.CARGB[1] = ((changeSlider.GetCurrent() * 0.1) - 5.0);
			matColors.SetParam("OverlayColor", CameraTool.CARGB );
		}
		else if ( changeSlider.GetName() == "camera_slider_color_b" ) 
		{
			CameraTool.CARGB[2] = ((changeSlider.GetCurrent() * 0.1) - 5.0);
			matColors.SetParam("OverlayColor", CameraTool.CARGB );
		}
		else if ( changeSlider.GetName() == "camera_slider_color_a" ) 
		{
			CameraTool.CARGB[3] = ((changeSlider.GetCurrent() * 0.05) - 4.0);
			matColors.SetParam("OverlayFactor", CameraTool.CARGB[3] );
		}
		else if ( changeSlider.GetName() == "camera_slider_view" )
		{
			CameraTool.VIEWDISTANCE = 100*changeSlider.GetCurrent();
			GetGame().GetWorld().SetPreferredViewDistance(CameraTool.VIEWDISTANCE);
		}
		return false;
	}

	override bool OnMouseWheel( Widget w, int x, int y, int wheel )
	{
		SliderWidget slider = widgetStore.GetSliderWidget( w.GetName() );

		if ( slider ) 
		{
			bool up = wheel < 0;
			int value = 1;

			if ( up ) value = -1;

			float current = slider.GetCurrent();
			slider.SetCurrent( current + value );

			OnChange( slider, x, y, false );
		}
		return false;
	}

	void UpdateSliders() 
	{
		string cameraTarget = "None";

		Object targetObject = CameraTool.Cast(COM_GetModuleManager().GetModule(CameraTool)).GetTargetObject();

		if ( targetObject ) 
		{
			cameraTarget = targetObject.GetType();
		}

		vector targetPos = CameraTool.Cast(COM_GetModuleManager().GetModule(CameraTool)).GetTargetPos();
		if ( targetPos != vector.Zero ) 
		{
			cameraTarget = COM_VectorToString( targetPos, 1 );
		}

		widgetStore.GetTextWidget("camera_target_txt").SetText("Target: " + cameraTarget );

		m_TxtCamBlur.SetText((CameraTool.CAMERA_BLUR * 100.0).ToString() + "%");

		string autoF = "";
		if ( CameraTool.CAMERA_AFOCUS ) 
		{
			autoF = " (AUTO)";
		}
		m_TxtCamDist.SetText(CameraTool.CAMERA_FDIST.ToString()+"m" + autoF);
		m_TxtCamFlen.SetText(CameraTool.CAMERA_FLENGTH.ToString());
		m_TxtCamFnear.SetText(CameraTool.CAMERA_FNEAR.ToString());
		m_TxtCamExp.SetText( CameraTool.EXPOSURE.ToString() );

		widgetStore.GetTextWidget("camera_slider_hue_value").SetText( CameraTool.HUESHIFT.ToString() ); // make script param that updates child textwidget of slider based on vvariable?
		widgetStore.GetTextWidget("camera_slider_rotblur_power_value").SetText( CameraTool.ROTBLUR.ToString() );
		widgetStore.GetTextWidget("camera_slider_rotblur_mindepth_value").SetText( CameraTool.MINDEPTH.ToString() );
		widgetStore.GetTextWidget("camera_slider_rotblur_maxdepth_value").SetText( CameraTool.MAXDEPTH.ToString() );
		widgetStore.GetTextWidget("camera_slider_radblur_value_x").SetText( CameraTool.RADBLURX.ToString() );
		widgetStore.GetTextWidget("camera_slider_radblur_value_y").SetText( CameraTool.RADBLURY.ToString() );
		widgetStore.GetTextWidget("camera_slider_radblur_value_offsetx").SetText( CameraTool.RADBLUROFFX.ToString() );
		widgetStore.GetTextWidget("camera_slider_radblur_value_offsety").SetText( CameraTool.RADBLUROFFY.ToString() );
		widgetStore.GetTextWidget("camera_slider_vign_value").SetText( CameraTool.VIGNETTE.ToString() );
		widgetStore.GetTextWidget("camera_slider_vign_value_r").SetText( CameraTool.VARGB[0].ToString() );
		widgetStore.GetTextWidget("camera_slider_vign_value_g").SetText( CameraTool.VARGB[1].ToString() );
		widgetStore.GetTextWidget("camera_slider_vign_value_b").SetText( CameraTool.VARGB[2].ToString() );
		widgetStore.GetTextWidget("camera_slider_color_value_r").SetText( CameraTool.CARGB[0].ToString() );
		widgetStore.GetTextWidget("camera_slider_color_value_g").SetText( CameraTool.CARGB[1].ToString() );
		widgetStore.GetTextWidget("camera_slider_color_value_b").SetText( CameraTool.CARGB[2].ToString() );
		widgetStore.GetTextWidget("camera_slider_color_value_a").SetText( CameraTool.CARGB[3].ToString() );
		widgetStore.GetTextWidget("camera_slider_view_value").SetText( CameraTool.VIEWDISTANCE.ToString() );

		// get child and update text?

		// Set slider current to value
		widgetStore.GetSliderWidget("camera_slider_hue").SetCurrent( (CameraTool.HUESHIFT + 5.0) / 0.1); // this could probably be moved to a proper system
		widgetStore.GetSliderWidget("camera_slider_rotblur_power").SetCurrent( (CameraTool.ROTBLUR / 0.5) / 0.01);
		widgetStore.GetSliderWidget("camera_slider_rotblur_mindepth").SetCurrent( (CameraTool.MINDEPTH / 0.01) / 2.5);
		widgetStore.GetSliderWidget("camera_slider_rotblur_maxdepth").SetCurrent( CameraTool.MAXDEPTH / 0.1);
		widgetStore.GetSliderWidget("camera_slider_radblur_x").SetCurrent( CameraTool.RADBLURX / 0.01);
		widgetStore.GetSliderWidget("camera_slider_radblur_y").SetCurrent( CameraTool.RADBLURY / 0.01);
		widgetStore.GetSliderWidget("camera_slider_radblur_offsetx").SetCurrent( CameraTool.RADBLUROFFX / 0.01);
		widgetStore.GetSliderWidget("camera_slider_radblur_offsety").SetCurrent( CameraTool.RADBLUROFFY / 0.01);
		widgetStore.GetSliderWidget("camera_slider_view").SetCurrent( CameraTool.VIEWDISTANCE / 100.0);

		// fk me ok im lazy. cbf doing this for all the sliders.

		TextWidget speedTxt = TextWidget.Cast(layoutRoot.FindAnyWidget( "camera_fov_speed_text" ));
		speedTxt.SetText("FOV Smooth: " + CameraTool.CAMERA_FOV_SPEED_MODIFIER );

		TextWidget smoothTxt = TextWidget.Cast(layoutRoot.FindAnyWidget( "camera_smooth_text" ));
		smoothTxt.SetText("Cam Smooth: " + CameraTool.CAMERA_SMOOTH );

		TextWidget sensTxt = TextWidget.Cast(layoutRoot.FindAnyWidget( "camera_msens_text" ));
		sensTxt.SetText("Cam Sens: " + CameraTool.CAMERA_MSENS );

		TextWidget flySpeed = TextWidget.Cast(layoutRoot.FindAnyWidget("camera_msmooth_text"));
		flySpeed.SetText("Fly smooth: " + (CameraTool.CAMERA_VELDRAG - 0.9) / 0.1);
	}	
}
// ===== core/modules/CameraTool/WidgetStore.c =====
class WidgetStore 
{
	private Widget layoutRoot;

	private ref array< TextWidget > textWidgets;
	private ref array< ButtonWidget > buttonWidgets;
	private ref array< EditBoxWidget > editBoxWidgets;
	private ref array< SliderWidget > sliderWidgets;
	private ref array< TextListboxWidget > textListBoxWidgets;

	private ref array< Widget > allWidgets;

	void WidgetStore( Widget root ) 
	{
		layoutRoot = root;

		textWidgets = new array< TextWidget >;
		buttonWidgets = new array < ButtonWidget >;
		editBoxWidgets = new array < EditBoxWidget >;
		sliderWidgets = new array < SliderWidget >;
		textListBoxWidgets = new array < TextListboxWidget >;
		allWidgets = new array < Widget >;

		Init();
	} 

	void ~WidgetStore() 
	{
	}

	void Init() 
	{
		SearchChildren( layoutRoot ); //recursion

		Print( "WidgetStore Init - Testing widget loading count: " + allWidgets.Count() );
	}

	void SearchChildren( Widget widget ) 
	{
		Widget child = widget.GetChildren();
		while ( child ) 
		{
			//Print( child.GetName() );
			allWidgets.Insert( child );
			AddWidget( child );
			SearchSibling( child );
			child = child.GetChildren();
		}
	}

	void SearchSibling( Widget widget )
	{
		Widget sibling = widget.GetSibling();
		while ( sibling ) 
		{
			//Print( sibling.GetName() );
			allWidgets.Insert( sibling );
			AddWidget( sibling );
			SearchChildren( sibling );
			sibling = sibling.GetSibling();
		}
	}

	void FindDuplicates() //debugging
	{
		int duplicates = 0;
		foreach( Widget widget : allWidgets ) 
		{
			string name = widget.GetName();

			int count = 0;
			foreach ( Widget check : allWidgets ) 
			{
				if ( name == check.GetName() ) 
				{
					count++;
				}
				if ( count > 1 ) 
				{
					Print( "Found duplicate: " + name );
					duplicates++;
					break;
				}
			}
		}
		Print( "Duplicates: " + duplicates );
	}

	void AddWidget( Widget widget ) 
	{
		if ( widget.IsInherited( TextWidget ) ) 
		{
			textWidgets.Insert( TextWidget.Cast(widget) );
		}
		if ( widget.IsInherited( ButtonWidget ) ) 
		{
			buttonWidgets.Insert( ButtonWidget.Cast(widget) );
		}
		if ( widget.IsInherited( EditBoxWidget ) ) 
		{
			editBoxWidgets.Insert( EditBoxWidget.Cast(widget) );
		}
		if ( widget.IsInherited( SliderWidget ) ) 
		{
			sliderWidgets.Insert( SliderWidget.Cast(widget) );
		}
		if ( widget.IsInherited( TextListboxWidget )) 
		{
			textListBoxWidgets.Insert( TextListboxWidget.Cast(widget) );
		}
	}

	Widget GetWidget( string name ) 
	{
		foreach( Widget widget : allWidgets ) 
		{
			if ( widget.GetName() == name ) 
			{
				return widget;
			}
		}
		return null;
	}

	TextWidget GetTextWidget( string name ) 
	{
		foreach( Widget widget : textWidgets ) 
		{
			if ( widget.GetName() == name ) 
			{
				return TextWidget.Cast(widget);
			}
		}
		return null;
	}

	ButtonWidget GetButtonWidget( string name ) 
	{
		foreach( Widget widget : buttonWidgets ) 
		{
			if ( widget.GetName() == name ) 
			{
				return ButtonWidget.Cast(widget);
			}
		}
		return null;
	}

	EditBoxWidget GetEditBoxWidget( string name ) 
	{
		foreach( Widget widget : editBoxWidgets ) 
		{
			if ( widget.GetName() == name ) 
			{
				return EditBoxWidget.Cast(widget);
			}
		}
		return null;
	}

	SliderWidget GetSliderWidget( string name ) 
	{
		foreach( Widget widget : sliderWidgets ) 
		{
			if ( widget.GetName() == name ) 
			{
				return SliderWidget.Cast(widget);
			}
		}
		return null;
	}

	TextListboxWidget GetListboxWidget( string name ) 
	{
		foreach( Widget widget : textListBoxWidgets ) 
		{
			if ( widget.GetName() == name ) 
			{
				return TextListboxWidget.Cast(widget);
			}
		}
		return null;
	}
}
#endif

#ifndef COM_MODULES_OLDLOADING
void RegisterModule()
{
    COM_GetModuleManager().RegisterModule( new CameraTool );
}
#endif
// ===== core/modules/ComEditor/module.c =====
#ifndef COM_MODULES_OLDLOADING
#endif
/*
    Define used for optional compilations
*/
#define MODULE_COM_EDITOR

/*
    Include of all .c files that belong to this module
*/
#ifdef COM_MODULES_OLDLOADING
// ===== core/modules/ComEditor/ObjectEditor.c =====
class ObjectEditor extends Module
{
	protected bool m_ObjectEditorActive = false;
	protected bool m_IsDragging;
	Object m_SelectedObject;
	Object building;

	// protected ref Scene active_Scene;
	autoptr array<Object> m_Objects = new array<Object>;

	string BASE_COM_DIR = "$saves:CommunityOfflineMode";
	string BASE_SCENE_DIR = BASE_COM_DIR + "\\Scenes";

	ref SceneManager sceneManager;
	SceneData currentSceneData;

	void ObjectEditor()
	{
//		MakeDirectory( BASE_COM_DIR );
//		MakeDirectory( BASE_SCENE_DIR );

		sceneManager = new SceneManager();
	}

	void ExportObjectLoad() // requested by robotstar78
	{
		// Copy paste your exported objects in here to make it look like the example below:
		/*
		SpawnObject( "Land_CementWorks_Hall2_Brick", "12941.188477 60.166824 5238.811523", "0.000000 0.000000 0.000000" );
		SpawnObject( "Land_CementWorks_Hall2_Brick", "12961.633789 54.630013 5247.288086", "0.000000 0.000000 0.000000" );
		SpawnObject( "Land_CementWorks_MillA", "12965.892578 70.050812 5223.445313", "0.000000 0.000000 0.000000" );
		SpawnObject( "Land_CementWorks_RotFurnace", "12957.807617 69.867195 5234.208984", "0.000000 0.000000 0.000000" );
		*/
	}

	void SpawnObject( string type, vector position, vector orientation )
	{
	    auto obj = GetGame().CreateObject( type, position );
	    obj.SetPosition( position );
	    obj.SetOrientation( orientation );
	    obj.SetOrientation( obj.GetOrientation() );
	    obj.Update();
	    obj.SetAffectPathgraph( true, false );
        GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().UpdatePathgraphRegionByObject, 100, false, obj );

	    m_Objects.Insert( obj );
	}

	void addObject( Object trackedObject )
	{
		m_Objects.Insert( trackedObject );
	}

	override void Init()
	{
		super.Init();

		LoadScene();
		ExportObjectLoad();
	}

	/**

		Works very similar to pluginkeybinding system with extended features such as multiple keybinds (mouse + keyboard) per function
		Please refrain from assigning same mouse keys to different functions even with more keybinds.
		ie: shift+click function vs click function will both call when pressing the former.

		WORK AROUND COULD BE TO MAKE PRIORITY TO FUNCTIONS THAT REQUIRE MORE KEYS/MOUSE BINDS
		IE: SHIFT+END TOGGLEOBJECTEDITOR VS CTRL+SHIFT+END SOMEOTHERFUNCTION
		THE FUNCTION WITH MORE BINDS SHOULD BE REGISTERED FIRST.

		TODO: MAKE SYSTEM LIKE THAT ^
		- dannydog

	**/
	override void RegisterKeyMouseBindings()
	{
//		KeyMouseBinding toggleEditor  = new KeyMouseBinding( GetModuleType(), "ToggleEditor" , "[Shift]+[End]" , "Toggle object editor."            );
		KeyMouseBinding objectSelect  = new KeyMouseBinding( GetModuleType(), "ClickObject"  , "Selects object on cursor.", true  );
		KeyMouseBinding objectDrag    = new KeyMouseBinding( GetModuleType(), "DragObject"   , "Drag objects on cursor.",   true  );
		KeyMouseBinding objectDelete  = new KeyMouseBinding( GetModuleType(), "DeleteObject" , "Deletes selected object.",  true  );
		KeyMouseBinding objectGround  = new KeyMouseBinding( GetModuleType(), "GroundObject" , "Snaps objects to ground.",  true  );
		KeyMouseBinding sceneSave     = new KeyMouseBinding( GetModuleType(), "ExportScene"  , "Saves current scene of objects", true);
		KeyMouseBinding tabFix        = new KeyMouseBinding( GetModuleType(), "TabFix"       , "Fixes issue with tabbing out of the game", true );
//		KeyMouseBinding objectScroll  = new KeyMouseBinding( GetModuleType(), "ScrollObject" , "[Shift][Ctrl][Alt]+(Wheel)" , "Raise or lower objects with mouse wheel as well as rotate.", true );

//		toggleEditor.AddBinding( KeyCode.KC_LSHIFT, KeyMouseBinding.KB_EVENT_HOLD    );
//		toggleEditor.AddBinding( KeyCode.KC_END   , KeyMouseBinding.KB_EVENT_RELEASE ); // Press END. Using Release prevents key HOLD spam from onKeyPress (could use ClearKey in onKeyPress however)
		objectDelete.AddBinding( "kDelete" ); // Pretty much making KB_EVENT_PRESS useless since you can just use KB_EVENT_HOLD instead.
		sceneSave.AddBinding( "kLControl" );
		sceneSave.AddBinding( "kS" );
		tabFix.AddBinding( "kLMenu" );

		objectSelect.AddBinding( "mBLeft" ); // Left Click
		objectDrag.AddBinding( "mBLeft" );
		objectDrag.SetActionType( KeyMouseActionType.HOLD );
		objectGround.AddBinding( "mBMiddle" );

		RegisterKeyMouseBinding( objectSelect );
		RegisterKeyMouseBinding( objectDrag   );
		RegisterKeyMouseBinding( objectDelete );
		RegisterKeyMouseBinding( objectGround );
		RegisterKeyMouseBinding( sceneSave );
		RegisterKeyMouseBinding( tabFix );
	}

    void TabFix()
    {
        m_SelectedObject = NULL;
    }

	void ExportScene()
	{
		string toCopy;
        toCopy += "//Spawn helper function\n";
        toCopy += "void SpawnObject( string type, vector position, vector orientation )\n";
        toCopy += "{\n";
        toCopy += "    auto obj = GetGame().CreateObject_WIP( type, position, ECE_CREATEPHYSICS );\n";
        toCopy += "    obj.SetFlags( EntityFlags.STATIC, false );\n";
        toCopy += "    obj.SetPosition( position );\n";
        toCopy += "    obj.SetOrientation( orientation );\n";
        toCopy += "    obj.SetOrientation( obj.GetOrientation() ); //Collision fix\n";
        toCopy += "    obj.Update();\n";
        toCopy += "    obj.SetAffectPathgraph( true, false );\n";
        toCopy += "    if( obj.CanAffectPathgraph() ) GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().UpdatePathgraphRegionByObject, 100, false, obj );\n";
        toCopy += "}\n";
        toCopy += "\n";
        toCopy += "//Your custom spawned objects\n";

		foreach( Object m_object : m_Objects )
		{
			toCopy = toCopy + "SpawnObject( \"" + m_object.GetType() + "\", \"" + COM_VectorToString( m_object.GetPosition() ) + "\", \"" + COM_VectorToString( m_object.GetOrientation() ) + "\" );\n";
		}

		GetGame().CopyToClipboard( toCopy );

		COM_Message( "Copied to clipboard" );
	}

	void SaveScene()
	{

		// loot spots
		// pumpkins = civilian
		// apple = medical
		// orange = industral
		// plum = military

/*
		SceneInfo sceneData = new SceneInfo("Test");
		// vector position = COM_GetPB().WorldToModel(); // origin point

		foreach( Object m_object : m_Objects )
		{
			vector pos = COM_GetPB().WorldToModel( m_object.GetPosition() );

			if ( m_object.GetType() != "Pumpkin" )
			{
				sceneData.AddObject( m_object, pos ); // add objects
			}
			// then add loot spots
		}

		foreach( Object m_objectt : m_Objects )
		{
			vector poss = COM_GetPB().WorldToModel( m_object.GetPosition() );

			if ( m_object.GetType() == "Pumpkin" )
			{
				sceneData.AddObject( m_objectt, poss ); // add objects
			}
			// then add loot spots
		}

		JsonFileLoader< SceneInfo >.JsonSaveFile( SCENE_DATA + "\\" + sceneData.GetName() + ".json", sceneData );

		*/

		/*
		string toCopy = "";

		foreach( Object m_object : m_Objects )
		{
			if ( m_object.GetType() == "Pot" ) //save loot positions
			{
				vector modelPos = m_SelectedObject.WorldToModel( m_object.GetPosition() );

				toCopy = toCopy + "{" + modelPos[0] + "," + modelPos[1] + "," + modelPos[2] + "},";
			}
		}
		toCopy = m_SelectedObject.GetType() + "[] = {" + toCopy + "};";

		GetGame().CopyToClipboard( toCopy );
		m_Objects.Clear();
		*/

        /*
		ref SceneSaveST scene = new SceneSaveST();
		scene.name = "latest";

		foreach( Object m_object : m_Objects )
		{
			ref Param objectParam = new Param3<string, vector, vector>( m_object.GetType(), m_object.GetPosition(), m_object.GetOrientation() );
			scene.m_SceneObjects.Insert( objectParam );
		}
		*/

		auto exportFile = OpenFile( "$saves:COMObjectEditorSave.json", FileMode.WRITE );

        if( !exportFile )
        {
            COM_Message( "Error writing COMObjectEditorSave.json. Current changes could not NOT be saved!!!" );
            return;
        }

        FPrint( exportFile, "{\"name\":\"latest\",\"m_SceneObjects\":[" );

        auto serial = new JsonSerializer;
        string file_content;
        foreach(int nObject, Object object : m_Objects )
        {
            auto objectParam = new Param3<string, vector, vector>( object.GetType(), object.GetPosition(), object.GetOrientation() );
            serial.WriteToString( objectParam, false, file_content );
            FPrint( exportFile, file_content );
            if( nObject < m_Objects.Count() - 1 ) FPrint( exportFile, "," );
        }

        FPrint( exportFile, "]}" );

        CloseFile( exportFile );

		COM_Message( "Saved objects to COMObjectEditorSave.json (User/Documents/DayZ)." );
//		JsonFileLoader< SceneSaveST >.JsonSaveFile( BASE_SCENE_DIR + "\\" + "latest.json", scene );
//		JsonFileLoader< SceneSaveST >.JsonSaveFile( "$saves:COMObjectEditorSave.json", scene );
	}

	void LoadScene()
	{
		SceneSaveST scene = new SceneSaveST();

//		JsonFileLoader<SceneSaveST>.JsonLoadFile( BASE_SCENE_DIR + "\\" + "latest.json", scene );
		JsonFileLoader<SceneSaveST>.JsonLoadFile( "$saves:COMObjectEditorSave.json", scene );

		foreach( auto param : scene.m_SceneObjects )
		{
			Object object = GetGame().CreateObject( param.param1, param.param2, false, false );
			object.SetOrientation( param.param3 );
			COM_ForceTargetCollisionUpdate( object );

			m_Objects.Insert( object );
		}
	}

	void EditorState( bool state )
	{
		if ( m_ObjectEditorActive == state )
		{
			return;
		}

		m_ObjectEditorActive = state;

		if ( m_ObjectEditorActive )
		{
			COM_GetPB().MessageStatus("Object Editor Enabled");
		}
		else
		{
			COM_GetPB().MessageStatus("Object Editor Disabled");
		}
	}

	void ToggleEditor()
	{
		m_ObjectEditorActive = !m_ObjectEditorActive;

		if ( m_ObjectEditorActive )
		{
			COM_GetPB().MessageStatus("Object Editor Enabled");
		}
		else
		{
			COM_GetPB().MessageStatus("Object Editor Disabled");
		}
	}

	bool IsEditing()
	{
		return m_ObjectEditorActive;
	}

	void SelectObject( Object object )
	{
		if ( ( ( m_SelectedObject != NULL ) && ( m_SelectedObject == object ) ) || object.IsInherited( PlayerBase ) )
		{
			return;
		}

		if ( COM_CTRL() ) 
		{
			vector modelPos = m_SelectedObject.WorldToModel( object.GetPosition() );

			ObjectInfoMenu.infoPosX.SetText( modelPos[0].ToString() );
			ObjectInfoMenu.infoPosY.SetText( modelPos[1].ToString() );
			ObjectInfoMenu.infoPosZ.SetText( modelPos[2].ToString() );

		}
		else 
		{
			ObjectInfoMenu.infoPosX.SetText( object.GetPosition()[0].ToString() );
			ObjectInfoMenu.infoPosY.SetText( object.GetPosition()[1].ToString() );
			ObjectInfoMenu.infoPosZ.SetText( object.GetPosition()[2].ToString() );

			ObjectInfoMenu.infoPosYaw.SetText( object.GetOrientation()[0].ToString() );
			ObjectInfoMenu.infoPosPitch.SetText( object.GetOrientation()[1].ToString() );
			ObjectInfoMenu.infoPosRoll.SetText( object.GetOrientation()[2].ToString() );
		}

		m_SelectedObject = object;
	}

	void DeselectObject()
	{
		m_SelectedObject = NULL;
	}

	void DragObject()
	{
		if ( !m_ObjectEditorActive )
		{
			return;
		}

		if ( m_SelectedObject )
		{
			vector dir = GetGame().GetPointerDirection();

			vector from = GetGame().GetCurrentCameraPosition();

			vector to = from + ( dir * 10000 );

			vector contact_pos;
			vector contact_dir;
			vector bbox;

			int contact_component;

			m_SelectedObject.GetCollisionBox( bbox );

			if ( DayZPhysics.RaycastRV( from, to, contact_pos, contact_dir, contact_component, NULL, m_SelectedObject, COM_GetPB(), false, false ) )
			{
				//vector oOrgPos = m_SelectedObject.GetPosition();
				//float fSurfaceHight = GetGame().SurfaceY( oOrgPos [ 0 ], oOrgPos [ 2 ] );

				//float nHightOffsetToGround = oOrgPos [ 1 ] - fSurfaceHight; // todo: store offset of existing objects for later use. for snap to ground perhaps?

				//contact_pos [ 1 ] = contact_pos [ 1 ] + nHightOffsetToGround;

				m_SelectedObject.SetPosition( contact_pos );
				m_SelectedObject.PlaceOnSurface();

				COM_ForceTargetCollisionUpdate( m_SelectedObject );

				ObjectInfoMenu.infoPosX.SetText( m_SelectedObject.GetPosition()[0].ToString() );
				ObjectInfoMenu.infoPosY.SetText( m_SelectedObject.GetPosition()[1].ToString() );
				ObjectInfoMenu.infoPosZ.SetText( m_SelectedObject.GetPosition()[2].ToString() );
			}
		}
	}

//	void ScrollObject( int state )
//	{
//		if ( !m_ObjectEditorActive )
//		{
//			return;
//		}
//		/*
//
//		if ( m_SelectedObject )
//		{
//			vector pos = m_SelectedObject.GetPosition();
//			vector ori = m_SelectedObject.GetOrientation();
//
//			bool up = state < 0;
//			int value = 1;
//			if ( up ) value = -1;
//
//			if ( COM_SHIFT() )
//			{
//				if ( ori[0] > 0 ) // seemless pitch change
//				{
//					value = -value;
//				}
//				ori[1] = ori[1] + value;
//
//				m_SelectedObject.SetOrientation( ori );
//			}
//			else if ( COM_CTRL() )
//			{
//				ori [ 0 ] = ori [ 0 ] + value;
//
//				m_SelectedObject.SetOrientation( ori );
//			}
//			else if ( COM_ALT() )
//			{
//				ori[ 2 ] = ori[ 2 ] + value;
//
//				m_SelectedObject.SetOrientation( ori );
//			}
//			else
//			{
//				pos [ 1 ] = pos [ 1 ] + value*0.05;
//
//				m_SelectedObject.SetPosition( pos );
//			}
//		}
//		*/
//	}

	void ClickObject()
	{
		if ( !m_ObjectEditorActive )
		{
			return;
		}
		Widget widgetCursor = GetGame().GetUIManager().GetWidgetUnderCursor();

		if ( widgetCursor && widgetCursor.GetName() != "EditorMenu" )
		{
			return;
		}

		vector dir = GetGame().GetPointerDirection();
		vector from = GetGame().GetCurrentCameraPosition();
		vector to = from + ( dir * 100 );

		set< Object > objects = COM_GetObjectsAt(from, to, COM_GetPB(), 0.5 );
		bool selected = false;
		//Print ("Building Type = " + building.GetType());

		if ( objects ) 
		{
			for ( int newObject = 0; ( ( newObject < objects.Count() ) && !selected ); ++newObject )
			{
				Object obj = objects.Get( newObject );

				if ( COM_CTRL() ) 
				{

					vector modelPos = obj.WorldToModel( m_SelectedObject.GetPosition() );

					ObjectInfoMenu.infoPosX.SetText( modelPos[0].ToString() );
					ObjectInfoMenu.infoPosY.SetText( modelPos[1].ToString() );
					ObjectInfoMenu.infoPosZ.SetText( modelPos[2].ToString() );
				}
				else 
				{
					SelectObject( obj );
					selected = true;

					ObjectInfoMenu.infoPosX.SetText( m_SelectedObject.GetPosition()[0].ToString() );
					ObjectInfoMenu.infoPosY.SetText( m_SelectedObject.GetPosition()[1].ToString() );
					ObjectInfoMenu.infoPosZ.SetText( m_SelectedObject.GetPosition()[2].ToString() );

					ObjectInfoMenu.infoPosYaw.SetText( m_SelectedObject.GetOrientation()[0].ToString() );
					ObjectInfoMenu.infoPosPitch.SetText( m_SelectedObject.GetOrientation()[1].ToString() );
					ObjectInfoMenu.infoPosRoll.SetText( m_SelectedObject.GetOrientation()[2].ToString() );
				}
			}
		}

		if ( !selected && m_SelectedObject )
		{
			COM_GetPB().MessageStatus("Current object deselected.");
			DeselectObject();
		}
	}

	void DeleteObject()
	{
		if ( !m_ObjectEditorActive )
		{
			return;
		}

		if ( m_SelectedObject )
		{
			m_Objects.RemoveItem(m_SelectedObject);
			m_SelectedObject.SetPosition(vector.Zero); // If object does not physically delete, teleport it to 0 0 0
			GetGame().ObjectDelete( m_SelectedObject );

			ObjectInfoMenu.UpdateObjectList();

			m_SelectedObject = NULL;
		}
	}

	void GroundObject()
	{
		if ( !m_ObjectEditorActive )
		{
			return;
		}

		if ( m_SelectedObject )
		{
			/*
			TStringArray sArray = new TStringArray;
			m_SelectedObject.GetSelectionList( sArray );

			foreach( string s : sArray )
			{
				Print( s );
				//GetPlayer().MessageStatus( s);
			}
			*/
			/*

			vector pos = m_SelectedObject.GetPosition();
			pos[1] = GetGame().SurfaceY(pos[0], pos[2]);

			vector clippingInfo[2];
			vector objectBBOX[2];

			m_SelectedObject.GetCollisionBox( objectBBOX );
			m_SelectedObject.ClippingInfo( clippingInfo );

			//float clipY = objectBBOX[1][1] / 2.0//- clippingInfo[0][1];
			//pos[1] = pos[1] + objectBBOX[1][1] - clipY;
			pos[1] = pos[1] + clippingInfo[1][1] / 2.0;//objectBBOX[0][1] - clipY

			m_SelectedObject.SetPosition(pos);
			*/
			//COM_SnapToGroundNew( m_SelectedObject );
			m_SelectedObject.PlaceOnSurface();
		}
	}
}

// ===== core/modules/ComEditor/gui/EditorMenu.c =====
class EditorMenu extends UIScriptedMenu 
{
    static const int MENU_ID = 133742;
    protected bool m_EscHeld = true;
    
    protected ButtonWidget m_ObjectButton;
	protected ButtonWidget m_PositionButton;
	protected ButtonWidget m_WeatherButton;
	protected ButtonWidget m_GameButton;
	protected ButtonWidget m_CameraButton;
	protected ButtonWidget m_ObjectEditorButton;
	
	protected Widget m_objectMenu;
	protected Widget m_weatherMenu;
	protected Widget m_positionMenu;
	protected Widget m_gameMenu;
	protected Widget m_objectInfoMenu;
	protected Widget m_cameraMenu;
    
	void EditorMenu()
	{
	    SetID(MENU_ID);
	}
    
    override int GetID()	
    {
		return MENU_ID;
	}

	override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\ComEditor\\gui\\layouts\\EditorMenu.layout" );

		m_ObjectButton   = ButtonWidget.Cast( layoutRoot.FindAnyWidget("objects_button") );
		m_PositionButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("position_button") );
		m_WeatherButton  = ButtonWidget.Cast( layoutRoot.FindAnyWidget("weather_button") );
		m_GameButton     = ButtonWidget.Cast( layoutRoot.FindAnyWidget("game_button") );
		m_CameraButton   = ButtonWidget.Cast( layoutRoot.FindAnyWidget("camera_button") );
		m_ObjectEditorButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget( "objectEditor_button" ));


		// object menu
		m_objectMenu   = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\ComEditor\\gui\\layouts\\ObjectMenu.layout", layoutRoot );
		m_weatherMenu  = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\ComEditor\\gui\\layouts\\WeatherMenu.layout", layoutRoot );
		m_positionMenu = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\Admintool\\gui\\layouts\\PositionMenu.layout", layoutRoot );
		m_gameMenu 	   = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\ComEditor\\gui\\layouts\\GameMenu.layout", layoutRoot );
		m_objectInfoMenu = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\ComEditor\\gui\\layouts\\ObjectEditorInfo.layout", layoutRoot );
		m_cameraMenu = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\CameraTool\\gui\\layouts\\CameraSettings.layout", layoutRoot );

        return layoutRoot;
	}
    
    override void OnShow()
    {
        super.OnShow();

        if ( layoutRoot )
        {
            layoutRoot.Show( true );
        }
    }

    override void LockControls()
    {
        super.LockControls();
        
        GetGame().GetInput().ChangeGameFocus(1);
    }
    
    override void UnlockControls()
    {
        super.UnlockControls();
        
        GetGame().GetInput().ChangeGameFocus(-1);
    }
    
    override void OnHide()
    {
        super.OnHide();

        ObjectEditor.Cast( COM_GetModuleManager().GetModule( ObjectEditor )).EditorState( false );

        if ( !CameraTool.Cast(COM_GetModuleManager().GetModule(CameraTool)).IsUsingCamera() ) 
        {
			COM_GetPB().GetInputController().OverrideMovementSpeed( false, 0 );
        }

        CameraSettings.CAMERA_ROT.Show( false );
        CameraSettings.CAMERA_PHI.Show( false );
    }
    
    override bool OnDoubleClick( Widget w, int x, int y, int button ) 
    {
    	if ( w == layoutRoot ) 
    	{
	    	ObjectMenu objectMenu;
	    	m_objectMenu.GetScript( objectMenu );
	    	string strSelection = objectMenu.GetCurrentSelection();

	    	if ( strSelection != "" ) 
	    	{
	    		bool ai = false;

	        	if ( GetGame().IsKindOf( strSelection, "DZ_LightAI" ) ) 
	        	{
	        		ai = true;
	        	}

	    		Object obj = GetGame().CreateObject( strSelection, COM_GetPointerPos(), true, ai );
	    		obj.PlaceOnSurface(); // reeeeeeeeeeeee
	    		COM_ForceTargetCollisionUpdate( obj );
	    		ObjectEditor.Cast(COM_GetModuleManager().GetModule( ObjectEditor )).SelectObject( obj );
	    		ObjectEditor.Cast(COM_GetModuleManager().GetModule( ObjectEditor )).addObject( obj );

	    		ObjectInfoMenu.listBox.AddItem(obj.GetType(), obj, 0);
	    	}
    	}

    	return false;
    }

    override bool OnClick( Widget w, int x, int y, int button )
	{
		PopupMenu popMenu;

		if ( w == m_ObjectButton ) 
		{
			m_objectMenu.GetScript( popMenu );
		}
		if ( w == m_PositionButton ) 
		{
			m_positionMenu.GetScript( popMenu );
		}

		if ( w == m_WeatherButton ) 
		{
			m_weatherMenu.GetScript( popMenu );
		}

		if ( w == m_GameButton ) 
		{
			m_gameMenu.GetScript( popMenu );
		}
		if ( w == m_ObjectEditorButton ) 
		{
			m_objectInfoMenu.GetScript( popMenu );

			ObjectEditor.Cast( COM_GetModuleManager().GetModule( ObjectEditor )).ToggleEditor();
		}

		if ( w == m_CameraButton ) 
		{
			m_cameraMenu.GetScript( popMenu );
		}

		if ( popMenu ) 
		{

			if ( popMenu.GetLayoutRoot().IsVisible() ) 
			{
				popMenu.GetLayoutRoot().Show( false );
				popMenu.OnHide();
			}
			else 
			{
				popMenu.GetLayoutRoot().Show( true );
				popMenu.OnShow();
			}

			SetButtonFocus( w );
			HideMenus( popMenu.GetLayoutRoot() );
		}

		return false;
	}

	void SetButtonFocus( Widget focus ) 
	{
		// todo change this so that its based on visible layout and not click event

		Widget m_ObjectButtonBkg   = layoutRoot.FindAnyWidget("objects_button_bkg");
		Widget m_PositionButtonBkg = layoutRoot.FindAnyWidget("position_button_bkg");
		Widget m_WeatherButtonBkg  = layoutRoot.FindAnyWidget("weather_button_bkg");
		Widget m_GameButtonBkg     = layoutRoot.FindAnyWidget("game_button_bkg");
		Widget m_ObjectEditorBkg   = layoutRoot.FindAnyWidget("objectEditor_button_bkg");
		Widget m_CameraButtonBkg   = layoutRoot.FindAnyWidget("camera_button_bkg");

		m_ObjectButtonBkg.SetColor(ARGB(0, 255, 255, 255)); // reset colors
		m_PositionButtonBkg.SetColor(ARGB(0, 255, 255, 255));
		m_WeatherButtonBkg.SetColor(ARGB(0, 255, 255, 255));
		m_GameButtonBkg.SetColor(ARGB(0, 255, 255, 255));
		m_ObjectEditorBkg.SetColor(ARGB(0, 255, 255, 255));
		m_CameraButtonBkg.SetColor(ARGB(0, 255, 255, 255));

		if ( m_ObjectButton == focus && m_objectMenu.IsVisible() ) 
		{
			m_ObjectButtonBkg.SetColor(ARGB(255, 255, 0, 0));
		}
		if ( m_WeatherButton == focus && m_weatherMenu.IsVisible() )
		{
			m_WeatherButtonBkg.SetColor(ARGB(255, 255, 0, 0));
		}
		if ( m_PositionButton == focus && m_positionMenu.IsVisible() )
		{
			m_PositionButtonBkg.SetColor(ARGB(255, 255, 0, 0));
		}
		if ( m_GameButton == focus && m_gameMenu.IsVisible() )
		{
			m_GameButtonBkg.SetColor(ARGB(255, 255, 0, 0));
		}
		if ( m_ObjectEditorButton == focus && m_objectInfoMenu.IsVisible() )
		{
			m_ObjectEditorBkg.SetColor(ARGB(255, 255, 0, 0));
		}
		if ( m_CameraButton == focus && m_cameraMenu.IsVisible() )
		{
			m_CameraButtonBkg.SetColor(ARGB(255, 255, 0, 0));
		}
	}

	void HideMenus( Widget focus ) 
	{
		if ( m_objectInfoMenu != focus && m_objectMenu != focus && m_objectMenu.IsVisible() ) 
		{
			m_objectMenu.Show(false);
		}
		if ( m_weatherMenu != focus && m_weatherMenu.IsVisible() ) 
		{
			m_weatherMenu.Show(false);
		}
		if ( m_positionMenu != focus && m_positionMenu.IsVisible() ) 
		{
			m_positionMenu.Show(false);
		}
		if ( m_gameMenu != focus && m_gameMenu.IsVisible() ) 
		{
			m_gameMenu.Show(false);
		}
		if ( m_cameraMenu != focus && m_cameraMenu.IsVisible() ) 
		{
			m_cameraMenu.Show(false);
		}
		if ( m_objectMenu != focus && m_objectInfoMenu != focus && m_objectInfoMenu.IsVisible() ) 
		{
			m_objectInfoMenu.Show(false);
			ObjectEditor.Cast( COM_GetModuleManager().GetModule( ObjectEditor )).ToggleEditor();
		}
	}

	override void Update( float timeslice ) 
	{
        if ( KeyState( KeyCode.KC_ESCAPE ) )
        {
            if ( !m_EscHeld )
            {
                m_EscHeld = true;
                GetGame().GetUIManager().Back();
                return;
            }
        }
        else
        {
            m_EscHeld = false;
        }

		//GetPlayer().MessageStatus( (GetMouseState( MouseState.RIGHT ) & MB_PRESSED_MASK).ToString() );

		if ( GetMouseState( MouseState.RIGHT ) & MB_PRESSED_MASK ) 
		{
			if ( GetGame().GetUIManager().IsCursorVisible() ) 
			{
				GetGame().GetUIManager().ShowUICursor( false );
				GetGame().GetInput().ResetGameFocus( );
			}
		}
		else
		{
			if ( !GetGame().GetUIManager().IsCursorVisible() ) 
			{
				GetGame().GetUIManager().ShowUICursor( true );
				GetGame().GetInput().ChangeGameFocus( 1 );
			}
		}
	}
}

// ===== core/modules/ComEditor/gui/ObjectMenu.c =====
class ObjectMenu extends PopupMenu
{
	protected TextListboxWidget m_classList;
	protected EditBoxWidget m_SearchBox;
	protected ButtonWidget m_btnSpawnGround;
	protected ButtonWidget m_btnSpawnCursor;
	protected ButtonWidget m_btnSpawnInventory;
	protected ButtonWidget m_btnCancel;
	protected EditBoxWidget m_QuantityItem;
	protected EditBoxWidget m_StateItem;

	private ItemPreviewWidget m_item_widget;
	protected EntityAI previewItem;
	private int m_characterRotationX;
	private int m_characterRotationY; // Borrowed from inspectmenu
	private int m_characterScaleDelta;
	private vector m_characterOrientation;

	protected EditBoxWidget m_editBox;

	protected autoptr array<string> m_excludeBroken = {
	    "GP25Base",
	    "GP25",
	    "GP25_Standalone",
	    "M203Base",
	    "M203",
	    "M203_Standalone",
	    "ItemOptics",
    };

	void ~ObjectMenu()
	{
		if ( previewItem ) 
		{
			GetGame().ObjectDelete( previewItem );
		}
	}

	override void Init()
	{
		m_classList = TextListboxWidget.Cast( layoutRoot.FindAnyWidget( "classlist" ) );
		m_SearchBox = EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "search_input" ) );
		m_btnSpawnGround = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_spawn_ground" ) );
		m_btnSpawnCursor = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_spawn_cursorpos" ) );
		m_btnSpawnInventory = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_spawn_inventory" ) );
		m_btnCancel = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_cancel" ) );

		m_QuantityItem = EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "quantity_items" ) );
		m_StateItem = EditBoxWidget.Cast( layoutRoot.FindAnyWidget( "state_items" ) );
		
		m_editBox = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("className_spawner_box"));
	}

	override void OnShow()
	{
		UpdateList( "All" );
	}

	override void OnHide() 
	{
		if ( previewItem ) 
		{
			GetGame().ObjectDelete( previewItem );
		}
	}

	override bool OnChange( Widget w, int x, int y, bool finished )
    {
        if ( w == m_SearchBox )
        {
            UpdateList( "All" );
            return true;
        }

        return false;
    }

    override bool OnMouseEnter( Widget w , int x, int y )
    {
        if ( w == m_SearchBox )
        {
            COM_GetPB().GetInputController().OverrideMovementSpeed( true, 0 );
        }
        return false;
    }

    override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
    {
        if ( w == m_SearchBox )
        {
            COM_GetPB().GetInputController().OverrideMovementSpeed( false, 0 );
        }
        return false;
    }

    override bool OnClick( Widget w, int x, int y, int button )
    {
        if (w.GetName().Contains("btn_filter"))
        {
            string buttonName = w.GetName();
            buttonName.Replace("btn_filter_", "");
            UpdateList(buttonName);
            return true;
        }

        if ((w != m_btnSpawnCursor) && (w != m_btnSpawnGround) && (w != m_btnSpawnInventory))
            return true;

        string strSelection = GetCurrentSelection();
        if (!strSelection)
            strSelection = GetEditBoxInput();
        
        if (!strSelection)
            return true;
          
        if( m_excludeBroken.Find(strSelection) != -1 ) 
            return true;
        
        strSelection.ToLower();

        ObjectEditor obEditor = ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor));

        EntityAI spawned;
        if (w == m_btnSpawnInventory)
        {
            spawned = g_Game.GetPlayer().GetInventory().CreateInInventory( strSelection );
        }
        else
        {
            vector position = COM_GetPB().GetPosition();
            if (w == m_btnSpawnCursor)
              position = COM_GetCursorPos();

            bool isAi = GetGame().IsKindOf(strSelection, "DZ_LightAI");
            spawned = EntityAI.Cast(g_Game.CreateObject(strSelection, position, true, isAi));
            spawned.PlaceOnSurface();
            COM_ForceTargetCollisionUpdate(spawned);

            obEditor.addObject(spawned);
            ObjectInfoMenu.listBox.AddItem(spawned.GetType(), spawned, 0);
        }

        ItemBase spawnedItem = ItemBase.Cast(spawned);
        if (spawnedItem)
        {
            float quantity01 = m_QuantityItem.GetText().ToFloat() / spawnedItem.GetQuantityMax();
            float health = m_StateItem.GetText().ToFloat();
            SetupSpawnedItem(spawnedItem, health, quantity01);
        }

        return true;
    }

    override bool OnItemSelected( Widget w, int x, int y, int row, int column, int oldRow, int oldColumn )
    {
        if ( w == m_classList )
        {
            string strSelection = GetCurrentSelection();
            m_characterOrientation = vector.Zero;

            if ( !m_item_widget )
            {
                Widget preview_panel = layoutRoot.FindAnyWidget("preview_pnl");

                if ( preview_panel )
                {
                    float width;
                    float height;
                    preview_panel.GetSize(width, height);

                    m_item_widget = ItemPreviewWidget.Cast( GetGame().GetWorkspace().CreateWidget(ItemPreviewWidgetTypeID, 0, 0, 1, 1, WidgetFlags.VISIBLE, ARGB(255, 255, 255, 255), 10, preview_panel) );
                }
            }

            if ( previewItem ) GetGame().ObjectDelete( previewItem );

            if( m_excludeBroken.Find( strSelection ) != -1 ) return false;

            Print(strSelection);

            previewItem = EntityAI.Cast(GetGame().CreateObject( strSelection, vector.Zero, false ));

            if( !previewItem ) return false;

            m_item_widget.SetItem( previewItem );
            m_item_widget.SetModelPosition( Vector(0,0,0.5) );
            m_item_widget.SetModelOrientation( m_characterOrientation );

            float itemx, itemy;
            m_item_widget.GetPos(itemx, itemy);

            m_item_widget.SetSize( 1.5, 1.5 );

            // align to center
            m_item_widget.SetPos( -0.225, -0.225 );

            // Calculate and Set QuantityMax on text box in quantity_items
            if (previewItem.IsInherited(Magazine))
            {
                m_QuantityItem.SetText(Magazine.Cast(previewItem).GetAmmoCount().ToString());
            }
            else if (previewItem.IsInherited(ItemBase))
            {
                m_QuantityItem.SetText(ItemBase.Cast(previewItem).GetQuantityMax().ToString());
            }
        }

        return true;
    }

	override bool OnMouseButtonDown( Widget w, int x, int y, int button ) 
	{
		if (w == m_item_widget)
		{
			GetGame().GetDragQueue().Call(this, "UpdateRotation");
			g_Game.GetMousePos(m_characterRotationX, m_characterRotationY);
			return true;
		}
		return false;
	}

	override bool OnMouseWheel( Widget w, int x, int y, int wheel ) 
	{
		if ( w == m_item_widget )
		{
			GetGame().GetDragQueue().Call(this, "UpdateScale");
			m_characterScaleDelta = wheel ;
		}
		return false;
	}

	void UpdateScale(int mouse_x, int mouse_y, int wheel, bool is_dragging) // Borrowed from inspect menu
	{
		float w, h, x, y;		
		m_item_widget.GetPos(x, y);
		m_item_widget.GetSize(w,h);
		w = w + ( m_characterScaleDelta / 4);
		h = h + ( m_characterScaleDelta / 4 );
		if ( w > 0.5 && w < 4 )
		{
			m_item_widget.SetSize( w, h );
	
			// align to center 
			int screen_w, screen_h;
			GetScreenSize(screen_w, screen_h);
			float new_x = x - ( m_characterScaleDelta / 8 );
			float new_y = y - ( m_characterScaleDelta / 8 );
			m_item_widget.SetPos( new_x, new_y );
		}
	}

	void UpdateRotation(int mouse_x, int mouse_y, bool is_dragging) // Borrowed from inspect menu
	{
		vector o = m_characterOrientation;
		o[0] = o[0] + (m_characterRotationY - mouse_y);
		o[1] = o[1] - (m_characterRotationX - mouse_x);
		
		m_item_widget.SetModelOrientation( o );
		
		if (!is_dragging)
		{
			m_characterOrientation = o;
		}
	}

    void UpdateList( string classFilter ) // All default
    {
        m_classList.ClearItems();

		TStringArray configs = new TStringArray;
		configs.Insert( CFG_VEHICLESPATH );
		configs.Insert( CFG_WEAPONSPATH );
		configs.Insert( CFG_MAGAZINESPATH );

        string strSearch = m_SearchBox.GetText();

		strSearch.ToLower();

		for ( int nConfig = 0; nConfig < configs.Count(); ++nConfig )
		{
			string strConfigPath = configs.Get( nConfig );

			int nClasses = g_Game.ConfigGetChildrenCount( strConfigPath );

			for ( int nClass = 0; nClass < nClasses; ++nClass )
			{
				string strName;

				g_Game.ConfigGetChildName( strConfigPath, nClass, strName );

				int scope = g_Game.ConfigGetInt( strConfigPath + " " + strName + " scope" );

				if ( scope == 0 )
				{
					continue;
				}

				auto strNameLower = strName;

				strNameLower.ToLower();

				if ( GetGame().IsKindOf( strNameLower, classFilter ) || classFilter == "All" ) // Fix for weapon_base not being child of "All"
				{
					if ( (strSearch != "" && (!strNameLower.Contains( strSearch ))) ) 
					{
						continue;
					}

					m_classList.AddItem( strName, NULL, 0 );
				}
			}
		}
    }

	string GetCurrentSelection()
	{
		if ( m_classList.GetSelectedRow() != -1 )
		{
			string result;
			m_classList.GetItemText( m_classList.GetSelectedRow(), 0, result );
			return result;
		}

		return "";
	}
	
	string GetEditBoxInput() 
	{
		return m_editBox.GetText();
	}
}
// ===== core/modules/AdminTool/gui/PositionMenu.c =====
class PositionMenu extends PopupMenu
{
	protected TextListboxWidget m_LstPositionList;
	protected EditBoxWidget m_TxtSelectedX;
	protected EditBoxWidget m_TxtSelectedY;
	protected EditBoxWidget m_TxtCurrentX;
	protected EditBoxWidget m_TxtCurrentY;
	protected ButtonWidget m_TeleportButton;
	protected ButtonWidget m_CancelButton;

	autoptr map< string, vector > Positions = new map< string, vector >;

    protected bool m_bOverCurrentPos;

	void PositionMenu()
	{
	    m_bOverCurrentPos = false;
        
        Positions.Insert( "Adamow", "3175.23 0 6805.85" );
        Positions.Insert( "Bagno", "11970.94 0 11220.80" );
        Positions.Insert( "Bielawa", "1392.87 0 9664.53" );
        Positions.Insert( "Borek", "9768.98 0 8497.65" );
        Positions.Insert( "Branzow Castle", "1075.98 0 11365.36" );
        Positions.Insert( "Brena", "6641.84 0 11130.16" );
        Positions.Insert( "Dambog", "554.79 0 1127.55" );
        Positions.Insert( "Drewniki", "5790.12 0 5087.65" );
        Positions.Insert( "Dolnik", "11390.37 0 612.61" );
        Positions.Insert( "Gieraltow", "11345.83 0 4397.68" );
        Positions.Insert( "Gliniska", "4993.88 0 9916.66" );
        Positions.Insert( "Grabinskie", "10959.82 0 10998.11" );
        Positions.Insert( "Hedrykow", "4507.86 0 4803.22" );
        Positions.Insert( "Hrud", "6479.31 0 9235.63" );
        Positions.Insert( "Huta", "5228.30 0 5567.43" );
        Positions.Insert( "Jantar", "3668.64 0 8889.91" );
        Positions.Insert( "Jezurow", "2104.29 0 2330.90" );
        Positions.Insert( "Karlin", "10078.41 0 6885.64" );
        Positions.Insert( "Kolembrody", "8417.17 0 11968.60" );
        Positions.Insert( "Kopa", "5555.70 0 8724.25" );
        Positions.Insert( "Krsnik", "7813.15 0 10044.03" );
        Positions.Insert( "Kulno", "11194.24 0 2453.57" );
        Positions.Insert( "Lipina", "5937.02 0 6777.02" );
        Positions.Insert( "Lukow", "3755.38 0 11720.90" );
        Positions.Insert( "Lembork", "8837.56 0 6614.49" );
        Positions.Insert( "Muratyn", "4586.78 0 6385.54" );
        Positions.Insert( "Nadbor", "6171.44 0 4091.37" );
        Positions.Insert( "Nidek", "6291.00 0 8077.26" );
        Positions.Insert( "Olszanka", "4757.12 0 7590.45" );
        Positions.Insert( "Piorun", "643.09 0 12108.47" );
        Positions.Insert( "Polana", "3362.77 0 2095.52" );
        Positions.Insert( "Radacz", "3962.44 0 7895.98" );
        Positions.Insert( "Radunin", "7291.96 0 6487.13" );
        Positions.Insert( "Rodzanica", "8900.85 0 2077.72" );
        Positions.Insert( "Roztoka", "7688.82 0 5238.29" );
        Positions.Insert( "Sitnickie", "11323.00 0 10018.24" );
        Positions.Insert( "Skala", "2997.42 0 1958.17" );
        Positions.Insert( "Sobotka", "6300.11 0 10161.06" );
        Positions.Insert( "Sowa", "11665.42 0 12014.70" );
        Positions.Insert( "Swarog", "4888.40 0 2211.34" );
        Positions.Insert( "Tarnow", "9268.88 0 10790.26" );
        Positions.Insert( "Topolin", "1873.00 0 7341.97" );
        Positions.Insert( "Tymbark", "2979.45 0 1129.89" );
        Positions.Insert( "Widok", "10256.57 0 2122.26" );
        Positions.Insert( "Wrzeszcz", "9037.59 0 4390.65" );
        Positions.Insert( "Zalesie", "865.18 0 5498.05" );
        Positions.Insert( "Zapadlisko", "8111.64 0 8693.00" );
	}

	void ~PositionMenu()
	{
	}

	override void Init()
	{
		m_LstPositionList = TextListboxWidget.Cast( layoutRoot.FindAnyWidget("tls_ppp_pm_positions_list") );
		m_TxtSelectedX = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("pnl_ppp_pm_selected_x_value") );
		m_TxtSelectedY = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("pnl_ppp_pm_selected_y_value") );
		m_TxtCurrentX = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("pnl_ppp_pm_current_x_value") );
		m_TxtCurrentY = EditBoxWidget.Cast( layoutRoot.FindAnyWidget("pnl_ppp_pm_current_y_value") );
		m_TeleportButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("btn_ppp_pm_teleport") );
		m_CancelButton = ButtonWidget.Cast( layoutRoot.FindAnyWidget("btn_ppp_pm_cancel") );

        for ( int nPosition = 0; nPosition < Positions.Count(); nPosition++ )
        {
            m_LstPositionList.AddItem( Positions.GetKey( nPosition ), NULL, 0 );
        }
	}

	override void OnShow()
	{
        vector player_pos = COM_GetPB().GetPosition();

        m_TxtCurrentX.SetText( player_pos[0].ToString() );
		m_TxtCurrentY.SetText( player_pos[2].ToString() );
	}

	override void OnHide()
	{

	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
        if ( w == m_TxtCurrentX || w == m_TxtCurrentY )
        {
            m_bOverCurrentPos = true;
        }

		return false;
	}

	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
        if ( w == m_TxtCurrentX || w == m_TxtCurrentY )
        {
            m_bOverCurrentPos = false;
        }

		return false;
	}

	override bool OnKeyPress( Widget w, int x, int y, int key )
	{
		if ( m_bOverCurrentPos )
		{
            m_TxtSelectedX.SetText( "" );
            m_TxtSelectedY.SetText( "" );
		}

		return false;
	}

	override bool OnClick( Widget w, int x, int y, int button )
	{
		if ( w == m_TeleportButton )
		{
		    float pos_x = 0;
            float pos_y = 0;

		    if( ( m_TxtSelectedX.GetText() != "" ) && ( m_TxtSelectedY.GetText() != "" ) )
		    {
	            pos_x = m_TxtSelectedX.GetText().ToFloat();
                pos_y = m_TxtSelectedY.GetText().ToFloat();
		    }
		    else if( ( m_TxtCurrentX.GetText() != "" ) && ( m_TxtCurrentY.GetText() != "" ) )
		    {
                pos_x = m_TxtCurrentX.GetText().ToFloat();
                pos_y = m_TxtCurrentY.GetText().ToFloat();
		    }
		    else
		    {
		        PlayerBase oPlayer = COM_GetPB();
		        oPlayer.MessageStatus( "Invalid teleportation position!" );

		        return true;
		    }

			vector vPlayerPos;
			vPlayerPos[0] = pos_x;
			vPlayerPos[2] = pos_y;

			vPlayerPos = COM_SnapToGround( vPlayerPos );

			COM_GetPB().SetPosition( vPlayerPos );

			return true;
		}

		return false;
	}

	override bool OnItemSelected( Widget w, int x, int y, int row, int column, int oldRow, int oldColumn )
	{
		vector position = "0 0 0";

		if( !Positions.Find( GetCurrentPositionName(), position ) )
		{
			position = "0 0 0";
		}

		m_TxtSelectedX.SetText( position[0].ToString() );
		m_TxtSelectedY.SetText( position[2].ToString() );

		return true;
	}

	string GetCurrentPositionName()
	{
		if ( m_LstPositionList.GetSelectedRow() != -1 )
		{
			string position_name;
			m_LstPositionList.GetItemText( m_LstPositionList.GetSelectedRow(), 0, position_name );
			return position_name;
		}

		return "";
	}
}
// ===== core/modules/ComEditor/gui/WeatherMenu.c =====
class WeatherMenu extends PopupMenu
{
	private static const int m_DaysInMonth [ 12 ] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    protected ButtonWidget m_BtnSave;
    protected ButtonWidget m_BtnCancel;
	protected TextWidget m_TxtWeatherTime;
	protected TextWidget m_TxtAirTemperature;
	protected SliderWidget m_SldStartTime;
	protected TextWidget m_TxtStartTimeValue;
	protected SliderWidget m_SldStartDay;
	protected TextWidget m_TxtStartDayValue;
	protected SliderWidget m_SldOvercast;
	protected TextWidget m_TxtOvercastValue;
	protected SliderWidget m_SldRain;
	protected TextWidget m_TxtRainValue;
	protected SliderWidget m_SldFog;
	protected TextWidget m_TxtFogValue;
	protected SliderWidget m_SldWindForce;
	protected TextWidget m_TxtWindForceValue;

	private int	  m_OrigYear;
	private int	  m_OrigMonth;
	private int	  m_OrigDay;
	private int	  m_OrigHour;
	private int	  m_OrigMinute;
	private float m_OrigOvercast;
	private float m_OrigRain;
	private float m_OrigFog;
	private float m_OrigWindForce;

	private int	  m_CurrYear;
	private int	  m_CurrMonth;
	private int	  m_CurrDay;
	private int	  m_CurrHour;
	private int	  m_CurrMinute;
	private float m_CurrOvercast;
	private float m_CurrRain;
	private float m_CurrFog;
	private float m_CurrWindForce;

	void WeatherMenu()
	{
	}

	void ~WeatherMenu()
	{
	}

	override void Init()
	{

        m_BtnSave			= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_save" ) );
        m_BtnCancel			= ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_cancel" ) );

		m_TxtWeatherTime	= TextWidget.Cast( layoutRoot.FindAnyWidget( "txt_ppp_st_w_time_value" ) );
		m_TxtAirTemperature	= TextWidget.Cast( layoutRoot.FindAnyWidget( "txt_ppp_st_w_air_temp_value" ) );

		m_SldStartTime		= SliderWidget.Cast( layoutRoot.FindAnyWidget( "sld_ppp_st_start_time" ) );
		m_TxtStartTimeValue	= TextWidget.Cast( layoutRoot.FindAnyWidget( "txt_ppp_st_start_time_value" ) );

		m_SldStartDay		= SliderWidget.Cast( layoutRoot.FindAnyWidget( "sld_ppp_st_start_day" ) );
		m_TxtStartDayValue	= TextWidget.Cast( layoutRoot.FindAnyWidget( "txt_ppp_st_start_day_value" ) );

		m_SldOvercast		= SliderWidget.Cast( layoutRoot.FindAnyWidget( "sld_ppp_st_overcast" ) );
		m_TxtOvercastValue	= TextWidget.Cast( layoutRoot.FindAnyWidget( "txt_ppp_st_overcast_value" ) );

		m_SldRain			= SliderWidget.Cast( layoutRoot.FindAnyWidget( "sld_ppp_st_rain" ) );
		m_TxtRainValue		= TextWidget.Cast( layoutRoot.FindAnyWidget( "txt_ppp_st_rain_value" ) );

		m_SldFog			= SliderWidget.Cast( layoutRoot.FindAnyWidget( "sld_ppp_st_fog" ) );
		m_TxtFogValue		= TextWidget.Cast( layoutRoot.FindAnyWidget( "txt_ppp_st_fog_value" ) );

		m_SldWindForce		= SliderWidget.Cast( layoutRoot.FindAnyWidget( "sld_ppp_st_wind_force" ) );
		m_TxtWindForceValue	= TextWidget.Cast( layoutRoot.FindAnyWidget( "txt_ppp_st_wind_force_value" ) );
	}

	override bool OnClick( Widget w, int x, int y, int button )
	{

		if ( w == m_BtnSave )
		{
			m_OrigYear = m_CurrYear;
			m_OrigMonth = m_CurrMonth;
			m_OrigDay = m_CurrDay;
			m_OrigHour = m_CurrHour;
			m_OrigMinute = m_CurrMinute;
			m_OrigOvercast = m_CurrOvercast;
			m_OrigRain = m_CurrRain;
			m_OrigFog = m_CurrFog;
			m_OrigWindForce = m_CurrWindForce;

			PluginSceneManager editor = PluginSceneManager.Cast( GetPlugin(PluginSceneManager) );
            if(editor)
            {
                editor.SetWeather(m_CurrOvercast, m_CurrRain, m_CurrFog, m_CurrWindForce);
                editor.SetDate(m_CurrYear, m_CurrMonth, m_CurrDay, m_CurrHour, m_CurrMinute);
            }

            GetGame().GetWeather().SetWindFunctionParams( m_OrigWindForce, m_CurrWindForce, 1 );

			return true;
		}

		return false;
	}

	override bool OnChange( Widget w, int x, int y, bool finished )
	{
		if ( w == m_SldStartTime )
		{
			float slider_value_start_time = m_SldStartTime.GetCurrent() * 0.01;
			float start_time_f = slider_value_start_time * 1439;
			int start_time = start_time_f;
			m_CurrHour = start_time / 60;
			m_CurrMinute = start_time % 60;

			UpdateSliderStartTime( m_CurrHour, m_CurrMinute );

			GetGame().GetWorld().SetDate( m_CurrYear, m_CurrMonth, m_CurrDay, m_CurrHour, m_CurrMinute );

			return true;
		}
		else if ( w == m_SldStartDay )
		{
			float slider_value_start_day = m_SldStartDay.GetCurrent();
			float start_day_f = slider_value_start_day * 3.64 + 1;
			int start_day = start_day_f;

			for ( int i = 0; i < 12; i++ )
			{
				int days = m_DaysInMonth[i];
				if ( start_day <= days )
				{
					m_CurrMonth = i+1;
					m_CurrDay = start_day;
					break;
				}
				else
				{
					start_day -= days;
				}
			}

			UpdateSliderStartDay( m_CurrMonth, m_CurrDay );

			GetGame().GetWorld().SetDate( m_CurrYear, m_CurrMonth, m_CurrDay, m_CurrHour, m_CurrMinute );

			return true;
		}
		else if ( w == m_SldOvercast )
		{
			UpdateSliderOvercast();

			m_CurrOvercast = m_SldOvercast.GetCurrent() * 0.01;
			GetGame().GetWeather().GetOvercast().Set( m_CurrOvercast, 0, 0 );
			GetGame().GetWorld().SetDate( m_CurrYear, m_CurrMonth, m_CurrDay, m_CurrHour, m_CurrMinute+5 );

			return true;
		}
		else if ( w == m_SldRain )
		{
			UpdateSliderRain();

			m_CurrRain = m_SldRain.GetCurrent() * 0.01;
			GetGame().GetWeather().GetRain().Set( m_CurrRain, 0, 0 );

			float density = m_CurrRain * m_CurrRain; 
			float threshold = 0.8; 
			float timeOut = 4000;

			GetGame().GetWeather().SetStorm(density, threshold, timeOut);

			return true;
		}
		else if ( w == m_SldFog )
		{
			UpdateSliderFog();

			m_CurrFog = m_SldFog.GetCurrent() * 0.01;
			GetGame().GetWeather().GetFog().Set( m_CurrFog, 0, 0 );

			return true;
		}
		else if ( w == m_SldWindForce )
		{
			UpdateSliderWindForce();

			m_CurrWindForce = m_SldWindForce.GetCurrent() * 0.01;

            GetGame().GetWeather().SetWindFunctionParams( m_OrigWindForce, m_CurrWindForce, 1 );

			return true;
		}

		return false;
	}

	override void OnShow()
	{
		GetGame().GetWorld().GetDate( m_OrigYear, m_OrigMonth, m_OrigDay, m_OrigHour, m_OrigMinute );

		Weather weather = GetGame().GetWeather();

		m_OrigOvercast = weather.GetOvercast().GetActual();
		m_OrigRain = weather.GetRain().GetActual();
		m_OrigFog = weather.GetFog().GetActual();
		m_OrigWindForce = weather.GetWindSpeed();

		m_CurrYear = m_OrigYear;
		m_CurrMonth = m_OrigMonth;
		m_CurrDay = m_OrigDay;
		m_CurrHour = m_OrigHour;
		m_CurrMinute = m_OrigMinute;
		m_CurrOvercast = m_OrigOvercast;
		m_CurrRain = m_OrigRain;
		m_CurrFog = m_OrigFog;
		m_CurrWindForce = m_OrigWindForce;

		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);

		ResetSliders();
	}

	override void OnHide()
	{
		/*
		Weather weather = GetGame().GetWeather();

		GetGame().GetWorld().SetDate( m_OrigYear, m_OrigMonth, m_OrigDay, m_OrigHour, m_OrigMinute );
		weather.GetOvercast().Set( m_OrigOvercast, 0, 1000 );
		weather.GetRain().Set( m_OrigRain, 0, 1000 );
		weather.GetFog().Set( m_OrigFog, 0, 1000 );
		weather.SetWindSpeed( m_OrigWindForce );

		*/
	}

	override void Update()
	{
		m_TxtWeatherTime.SetText( GetGame().GetWeather().GetTime().ToString() );
		// m_TxtAirTemperature.SetText( GetGame().GetWeather().GetAirTemperature().ToString() );
	}

	void ResetSliders()
	{
		int year, month, day, hour, minute;

		GetGame().GetWorld().GetDate( year, month, day, hour, minute );

		m_SldStartTime.SetCurrent( ((hour * 60) + minute) / 14.39 );

		UpdateSliderStartTime( hour, minute );


		float start_day = day;

		int month_tmp = month;

		while ( month_tmp > 1 )
		{
			month_tmp--;
			start_day += m_DaysInMonth[month];
		}

		m_SldStartDay.SetCurrent( start_day / 3.64 );

		UpdateSliderStartDay( month, day );

		Weather weather = GetGame().GetWeather();

		m_SldOvercast.SetCurrent(weather.GetOvercast().GetActual() * 100);
		UpdateSliderOvercast();

		m_SldRain.SetCurrent(weather.GetRain().GetActual() * 100);
		UpdateSliderRain();

		m_SldFog.SetCurrent(weather.GetFog().GetActual() * 100);
		UpdateSliderFog();

		float slider_wind_value = ( weather.GetWindSpeed() / weather.GetWindMaximumSpeed() ) * 100;

		if( weather.GetWindSpeed() < 0.01 )
		{
		    slider_wind_value = 0.0;
		}

		m_SldWindForce.SetCurrent( slider_wind_value );

		UpdateSliderWindForce();
	}

	void UpdateSliderStartTime( int hour, int minute )
	{
		string label_text = hour.ToStringLen(2) + ":" + minute.ToStringLen(2);
		m_TxtStartTimeValue.SetText( label_text );
	}

	void UpdateSliderStartDay( int month, int day )
	{
		string label_text = day.ToString() + "." + month.ToString() + ". " + m_CurrYear.ToString();
		m_TxtStartDayValue.SetText( label_text );
	}

	void UpdateSliderOvercast()
	{
		string label_text = m_SldOvercast.GetCurrent().ToString()+"%";
		m_TxtOvercastValue.SetText( label_text );
	}

	void UpdateSliderRain()
	{
		string label_text = m_SldRain.GetCurrent().ToString()+"%";
		m_TxtRainValue.SetText( label_text );
	}

	void UpdateSliderFog()
	{
		string label_text = m_SldFog.GetCurrent().ToString()+"%";
		m_TxtFogValue.SetText( label_text );
	}

	void UpdateSliderWindForce()
	{
		string label_text = m_SldWindForce.GetCurrent().ToString()+"%";
		m_TxtWindForceValue.SetText( label_text );
	}
}
// ===== core/modules/ComEditor/gui/GameMenu.c =====
class GameMenu extends PopupMenu
{
	TextListboxWidget m_gameScriptList;
	Widget 			  m_checkboxPanel;
	ButtonWidget 	  m_gameScriptButton;

	protected ref map< string, string > checkBoxMap = new map< string, string >; // store widget name

	string checkboxLayout = "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\ComEditor\\gui\\layouts\\CheckboxTemplate.layout";

	override void Init()
	{
		m_checkboxPanel = layoutRoot.FindAnyWidget("game_checkbox_panel");
		m_gameScriptList = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("game_list_box"));
		m_gameScriptButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("game_script_button"));

		m_gameScriptList.AddItem( "Spawn HMMWV",     new Param1< string >( "SpawnHMMWV" ),     0 );
		m_gameScriptList.AddItem( "Spawn Hatchback", new Param1< string >( "SpawnHatchback" ), 0 );
		m_gameScriptList.AddItem( "Spawn Sedan", 	 new Param1< string >( "SpawnSedan" ), 	   0 );
        m_gameScriptList.AddItem( "Spawn Gunter 2",  new Param1< string >( "SpawnGunter" ),    0 );
        m_gameScriptList.AddItem( "Spawn Sarka",     new Param1< string >( "SpawnSarka" ),     0 );
//      m_gameScriptList.AddItem( "Spawn V3S",		 new Param1< string >( "SpawnV3S" ), 	   0 );
//		m_gameScriptList.AddItem( "Spawn V3S_Cargo", new Param1< string >( "SpawnV3SCargo" ),  0 );
//		m_gameScriptList.AddItem( "Spawn Bus", 		 new Param1< string >( "SpawnBus" ), 	   0 );
//		m_gameScriptList.AddItem( "Spawn Van",	     new Param1< string >( "SpawnVan" ), 	   0 );

		AddAutoVehicles();

		CheckBoxWidget checkBoxGodmode = CheckBoxWidget.Cast(GetGame().GetWorkspace().CreateWidgets( checkboxLayout, m_checkboxPanel ));
		checkBoxGodmode.SetName( "Godmode" );
		checkBoxGodmode.SetText( "Godmode" );

//		CheckBoxWidget checkBoxAiming = CheckBoxWidget.Cast(GetGame().GetWorkspace().CreateWidgets( checkboxLayout, m_checkboxPanel ));
//		checkBoxAiming.SetName("OldAiming");
//		checkBoxAiming.SetText("Old Aiming");
//
//		CheckBoxWidget checkBoxLaser = CheckBoxWidget.Cast(GetGame().GetWorkspace().CreateWidgets( checkboxLayout, m_checkboxPanel ));
//		checkBoxLaser.SetName("LaserPointer");
//		checkBoxLaser.SetText("Laser Pointer");

		checkBoxMap.Insert( checkBoxGodmode.GetName(), "ToggleGodMode" );
//		checkBoxMap.Insert( checkBoxAiming.GetName(), "ToggleOldAiming" );
//		checkBoxMap.Insert( checkBoxLaser.GetName(), "ToggleLaser" );
	}

	override void OnShow()
	{
		// Update checkbox checked
		UpdateCheckBoxes();
	}

	void UpdateCheckBoxes() 
	{
		foreach( string widgetName, string function : checkBoxMap ) 
		{
			Widget widget = m_checkboxPanel.FindWidget( widgetName );

			if ( widget.IsInherited( CheckBoxWidget )) 
			{
				CheckBoxWidget checkbox = CheckBoxWidget.Cast( widget );
				bool checked;
				GetGame().GameScript.CallFunction( this , function , checked, NULL );
				checkbox.SetChecked( checked );
			}
		}
	}

	bool ToggleOldAiming( CheckBoxWidget widget )
	{
		if ( widget ) // Temp work around. I'm lazy xd
		{
			m_COM_OldAiming = !m_COM_OldAiming;

			COM_GetPB().OverrideShootFromCamera( !m_COM_OldAiming );
		}
		return m_COM_OldAiming;
	}

	bool ToggleGodMode( CheckBoxWidget widget ) 
	{
		if ( widget ) 
		{
			m_COM_GodMode = !m_COM_GodMode;

			COM_GetPB().SetAllowDamage( !m_COM_GodMode );
		}
		return m_COM_GodMode;
	}

	bool ToggleLaser( CheckBoxWidget widget ) 
	{
		if ( widget ) 
		{
			COM_bc_Visible = !COM_bc_Visible;
		}
		return COM_bc_Visible;
	}

	override bool OnClick( Widget w, int x, int y, int button )
	{

		string param;
		Param1<string> param1;

		if ( w == m_gameScriptButton ) 
		{
			int selectRow = m_gameScriptList.GetSelectedRow();

			if ( selectRow == -1 ) return false;

			m_gameScriptList.GetItemData( selectRow, 0, param1 );

			if ( param1 ) 
			{
				GetGame().GameScript.CallFunction( this , param1.param1 , NULL, 0 );
			}
		}

		if ( w.IsInherited( CheckBoxWidget ) ) 
		{
			param = checkBoxMap.Get( w.GetName() );

			if ( param ) 
			{
				GetGame().GameScript.CallFunction( this , param , NULL, CheckBoxWidget.Cast( w ) );
			}
		}

		return false;
	}

    void SpawnHMMWV() 
    {
        TStringArray attArr = {
        "HeadlightH7", "HeadlightH7", "CarBattery", "GlowPlug", 
        "Offroad_02_Wheel", "Offroad_02_Wheel", "Offroad_02_Wheel", "Offroad_02_Wheel", "Offroad_02_Wheel",
        "Offroad_02_Hood", "Offroad_02_Trunk", 
        "Offroad_02_Door_1_1", "Offroad_02_Door_1_2", "Offroad_02_Door_2_1", "Offroad_02_Door_2_2",
        };

        SpawnVehicle( "Offroad_02", attArr );
    }

	void SpawnHatchback() 
	{
		TStringArray attArr = {
		"HeadlightH7", "HeadlightH7",
		"HatchbackWheel", "HatchbackWheel", "HatchbackWheel", "HatchbackWheel",
		"CarBattery", "CarRadiator", "EngineBelt", "SparkPlug", "HatchbackHood",
		"HatchbackTrunk", "HatchbackDoors_Driver", "HatchbackDoors_CoDriver",
		};

		SpawnVehicle( "OffroadHatchback", attArr );
	}

	void SpawnSedan() 
	{
		TStringArray attArr = {
		"HeadlightH7", "HeadlightH7",
		"CivSedanWheel", "CivSedanWheel", "CivSedanWheel", "CivSedanWheel",
		"CarBattery", "CarRadiator","EngineBelt", "SparkPlug","CivSedanHood",
		"CivSedanTrunk", "CivSedanDoors_Driver","CivSedanDoors_CoDriver",
		"CivSedanDoors_BackLeft", "CivSedanDoors_BackRight",
		};

		SpawnVehicle( "CivilianSedan", attArr );
	}

	void SpawnGunter()
	{
		TStringArray attArr = {
		"HeadlightH7", "HeadlightH7", "CarBattery", "CarRadiator", "SparkPlug", "Hatchback_02_Door_1_1",
		"Hatchback_02_Door_1_2", "Hatchback_02_Door_2_1","Hatchback_02_Door_2_2",
		"Hatchback_02_Trunk", "Hatchback_02_Hood", "Hatchback_02_Wheel", "Hatchback_02_Wheel",
		"Hatchback_02_Wheel", "Hatchback_02_Wheel", "CivSedanDoors_BackRight",
		};

		SpawnVehicle( "Hatchback_02", attArr );
	}

	void SpawnSarka()
	{
		TStringArray attArr = {
		"HeadlightH7", "HeadlightH7", "CarBattery", "CarRadiator", "SparkPlug", "Sedan_02_Hood",
		"Sedan_02_Hood","Sedan_02_Trunk","Sedan_02_Door_1_1",
		"Sedan_02_Door_2_1","Sedan_02_Door_1_2","Sedan_02_Door_2_2","Sedan_02_Wheel",
		"Sedan_02_Wheel","Sedan_02_Wheel","Sedan_02_Wheel",
		};

		SpawnVehicle( "Sedan_02", attArr );
	}

	void SpawnV3SCargo() 
	{
		TStringArray attArr = {
		"V3SWheel","V3SWheel", "V3SWheel","V3SWheel", "V3SWheelDouble","V3SWheelDouble", "V3SWheelDouble","V3SWheelDouble",
		"TruckBattery","TruckRadiator","EngineBelt","GlowPlug","V3SHood",
		"V3SDoors_Driver_Orange","V3SDoors_CoDriver_Orange",
		};

		SpawnVehicle( "V3S_Cargo_Blue", attArr );
	}

	void SpawnV3S() 
	{
		TStringArray attArr = {
		"V3SWheel","V3SWheel", "V3SWheel","V3SWheel", "V3SWheelDouble","V3SWheelDouble", "V3SWheelDouble","V3SWheelDouble",
		"TruckBattery","TruckRadiator","EngineBelt","GlowPlug","V3SHood",
		"V3SDoors_Driver_Orange","V3SDoors_CoDriver_Orange",
		};

		SpawnVehicle( "V3S_Chassis_Blue", attArr );
	}

	void SpawnBus() 
	{
		TStringArray attArr = {
		"TransitBusWheel","TransitBusWheel", "TransitBusWheelDouble","TransitBusWheelDouble",
		"TruckBattery","TruckRadiator","EngineBelt","GlowPlug","BusHood",
		"BusDoors_Left","BusDoors_Right", "BusDoors_Left","BusDoors_Right", "BusDoors_Left","BusDoors_Right",
		};

		SpawnVehicle( "TransitBus", attArr );
	}

	void SpawnVan() 
	{
		TStringArray attArr = {
			"CivVanWheel","CivVanWheel","CivVanWheel","CivVanWheel",
			"CarBattery","CarRadiator","EngineBelt","SparkPlug","CivVanTrunk",
			"CivVanDoors_Driver","CivVanDoors_CoDriver","CivVanDoors_BackRight",
			"CivVanDoors_TrumpDown", "CivVanDoors_TrumpUp",
		};

		SpawnVehicle( "CivilianVan", attArr );
	}

	// ================= VEHICULOS COMPLETOS (autodetectados) =================
	// Detecta todos los vehiculos (coches, motos, barcos) del juego, les pone todas las
	// piezas de sus ranuras y llena combustible, aceite, frenos y refrigerante.
	protected ref map< string, ref TStringArray > m_COM_SlotItems;

	void AddAutoVehicles()
	{
		TStringArray autoVehicles = COM_GetAllVehicleClasses();

		m_gameScriptList.AddItem( "---- VEHICULOS COMPLETOS ----", NULL, 0 );
		m_gameScriptList.AddItem( "Spawn TODOS en fila", new Param1< string >( "SpawnAllVehicles" ), 0 );

		for ( int av = 0; av < autoVehicles.Count(); av++ )
		{
			m_gameScriptList.AddItem( "Full " + autoVehicles.Get( av ), new Param1< string >( "SpawnSelectedVehicle" ), 0 );
		}
	}

	TStringArray COM_GetAllVehicleClasses()
	{
		TStringArray found = new TStringArray;
		string cfgPath = "CfgVehicles";
		string className;
		int cfgCount = GetGame().ConfigGetChildrenCount( cfgPath );

		for ( int ci = 0; ci < cfgCount; ci++ )
		{
			GetGame().ConfigGetChildName( cfgPath, ci, className );

			if ( GetGame().ConfigGetInt( cfgPath + " " + className + " scope" ) != 2 ) continue;

			if ( GetGame().IsKindOf( className, "Transport" ) || GetGame().IsKindOf( className, "Car" ) || GetGame().IsKindOf( className, "CarScript" ) || GetGame().IsKindOf( className, "Boat" ) || GetGame().IsKindOf( className, "BoatScript" ) || GetGame().IsKindOf( className, "Motorbike" ) )
			{
				found.Insert( className );
			}
		}

		return found;
	}

	void COM_BuildSlotMap()
	{
		if ( m_COM_SlotItems ) return;

		m_COM_SlotItems = new map< string, ref TStringArray >;

		string mapCfg = "CfgVehicles";
		string itemName;
		string itemLower;
		string singleSlot;
		string slotKey;
		TStringArray itemSlots;
		TStringArray slotList;
		int mapCount = GetGame().ConfigGetChildrenCount( mapCfg );

		for ( int mi = 0; mi < mapCount; mi++ )
		{
			GetGame().ConfigGetChildName( mapCfg, mi, itemName );

			if ( GetGame().ConfigGetInt( mapCfg + " " + itemName + " scope" ) != 2 ) continue;

			itemLower = itemName;
			itemLower.ToLower();
			if ( itemLower.Contains( "ruined" ) ) continue;

			itemSlots = new TStringArray;
			GetGame().ConfigGetTextArray( mapCfg + " " + itemName + " inventorySlot", itemSlots );

			if ( itemSlots.Count() == 0 )
			{
				singleSlot = "";
				GetGame().ConfigGetText( mapCfg + " " + itemName + " inventorySlot", singleSlot );
				if ( singleSlot != "" ) itemSlots.Insert( singleSlot );
			}

			for ( int ki = 0; ki < itemSlots.Count(); ki++ )
			{
				slotKey = itemSlots.Get( ki );

				if ( !m_COM_SlotItems.Contains( slotKey ) )
				{
					slotList = new TStringArray;
					m_COM_SlotItems.Insert( slotKey, slotList );
				}

				m_COM_SlotItems.Get( slotKey ).Insert( itemName );
			}
		}
	}

	// Llena TODOS los liquidos al maximo de la capacidad real de cada vehiculo.
	// Motos y barcos: no son Car. Se llama por nombre en tiempo de ejecucion (no rompe la compilacion si el metodo no existe).
	void COM_FillNonCarFuel( Object nonCarObj, bool report )
	{
		float nonCarCap = 0;
		float nonCarFrac = -1;
		float nonCarDummy;
		bool nonCarOk = false;

		ScriptModule nonCarScript = GetGame().GameScript;
		if ( nonCarScript )
		{
			nonCarScript.CallFunctionParams( nonCarObj, "GetFluidCapacity", nonCarCap, new Param1<int>( 0 ) );
			if ( nonCarCap <= 0 ) nonCarCap = 100;
			nonCarOk = nonCarScript.CallFunctionParams( nonCarObj, "Fill", nonCarDummy, new Param2<int, float>( 0, nonCarCap ) );
			nonCarScript.CallFunctionParams( nonCarObj, "GetFluidFraction", nonCarFrac, new Param1<int>( 0 ) );
		}

		if ( report )
		{
			if ( nonCarOk )
			{
				float nonCarPercent = Math.Round( nonCarFrac * 100 );
				COM_Message( nonCarObj.GetType() + " (" + nonCarObj.ClassName() + ") combustible: " + nonCarPercent.ToString() + "% (capacidad " + nonCarCap.ToString() + ")" );
			}
			else
			{
				COM_Message( nonCarObj.GetType() + " (" + nonCarObj.ClassName() + "): no se encontro Fill() en este vehiculo" );
			}
		}
	}

	void COM_FillVehicleFluids( Object vehicleObj, bool report )
	{
		if ( !vehicleObj ) return;

		Car fillCar = Car.Cast( vehicleObj );
		if ( !fillCar )
		{
			COM_FillNonCarFuel( vehicleObj, report );
			return;
		}

		fillCar.Fill( CarFluid.FUEL,    fillCar.GetFluidCapacity( CarFluid.FUEL ) );
		fillCar.Fill( CarFluid.OIL,     fillCar.GetFluidCapacity( CarFluid.OIL ) );
		fillCar.Fill( CarFluid.BRAKE,   fillCar.GetFluidCapacity( CarFluid.BRAKE ) );
		fillCar.Fill( CarFluid.COOLANT, fillCar.GetFluidCapacity( CarFluid.COOLANT ) );

		if ( report )
		{
			float fuelPercent = Math.Round( fillCar.GetFluidFraction( CarFluid.FUEL ) * 100 );
			COM_Message( fillCar.GetType() + " combustible: " + fuelPercent.ToString() + "%" );
		}
	}

	Object COM_SpawnFullVehicle( string vehicleClass, vector position )
	{
		Object vehicleObj = GetGame().CreateObject( vehicleClass, position, false, false );
		if ( !vehicleObj ) return NULL;

		EntityAI vehicleEntity = EntityAI.Cast( vehicleObj );
		if ( !vehicleEntity ) return vehicleObj;

		COM_BuildSlotMap();

		TStringArray vehicleSlots = new TStringArray;
		TStringArray slotCandidates;
		string slotName;
		GetGame().ConfigGetTextArray( "CfgVehicles " + vehicleClass + " attachments", vehicleSlots );

		for ( int si = 0; si < vehicleSlots.Count(); si++ )
		{
			slotName = vehicleSlots.Get( si );

			if ( !m_COM_SlotItems.Contains( slotName ) ) continue;

			slotCandidates = m_COM_SlotItems.Get( slotName );
			if ( slotCandidates.Count() > 0 )
			{
				vehicleEntity.GetInventory().CreateAttachment( slotCandidates.Get( 0 ) );
			}
		}

		// Llenado inmediato y repetido 1.5 s despues (el juego puede reiniciar los liquidos al crear el vehiculo)
		COM_FillVehicleFluids( vehicleObj, false );
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( COM_FillVehicleFluids, 1500, false, vehicleObj, true );

		COM_Message( "Spawned " + vehicleClass );

		return vehicleObj;
	}

	void SpawnSelectedVehicle()
	{
		int selectedRow = m_gameScriptList.GetSelectedRow();
		if ( selectedRow == -1 ) return;

		string rowText;
		m_gameScriptList.GetItemText( selectedRow, 0, rowText );
		rowText.Replace( "Full ", "" );

		if ( rowText == "" ) return;

		COM_SpawnFullVehicle( rowText, COM_GetCursorPos() );
	}

	void SpawnAllVehicles()
	{
		TStringArray allVehicles = COM_GetAllVehicleClasses();
		vector basePos = COM_GetCursorPos();
		vector spawnPos;

		int gridCol = 0;
		int gridRow = 0;

		for ( int vi = 0; vi < allVehicles.Count(); vi++ )
		{
			spawnPos = basePos;
			spawnPos[0] = basePos[0] + gridCol * 9;
			spawnPos[2] = basePos[2] + gridRow * 14;
			spawnPos[1] = GetGame().SurfaceY( spawnPos[0], spawnPos[2] );

			COM_SpawnFullVehicle( allVehicles.Get( vi ), spawnPos );

			gridCol = gridCol + 1;
			if ( gridCol >= 8 )
			{
				gridCol = 0;
				gridRow = gridRow + 1;
			}
		}
	}
	// ================= FIN VEHICULOS COMPLETOS =================

	void SpawnVehicle( string vehicle, TStringArray attachments) 
	{
		Car oCar = Car.Cast( GetGame().CreateObject( vehicle, COM_GetCursorPos(), false, false ) );

		for (int j = 0; j < attachments.Count(); j++) { oCar.GetInventory().CreateAttachment( attachments.Get(j) ); }

		COM_FillVehicleFluids( oCar, false );
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( COM_FillVehicleFluids, 1500, false, oCar, true );

		//oCar.EngineStart();
		//oCar.SwitchLights();
	}
};

// ===== core/modules/ComEditor/gui/PopupMenu.c =====
class PopupMenu extends ScriptedWidgetEventHandler 
{
	protected Widget layoutRoot;

	void PopupMenu() 
	{

	}

	void ~PopupMenu() 
	{
	}

	void OnWidgetScriptInit( Widget w )
	{
		layoutRoot = w;
		layoutRoot.SetHandler( this );

		Init();
	}

	void Init() 
	{

	}

	void OnShow()
	{
	}

	void OnHide() 
	{
	}

	void Update() 
	{
		
	}

	Widget GetLayoutRoot() 
	{
		return layoutRoot;
	}

}
// ===== core/modules/ComEditor/gui/ObjectInfoMenu.c =====
class ObjectInfoMenu extends PopupMenu
{
	static EditBoxWidget infoPosX;
	static EditBoxWidget infoPosY;
	static EditBoxWidget infoPosZ;

	static EditBoxWidget infoPosYaw;
	static EditBoxWidget infoPosPitch;
	static EditBoxWidget infoPosRoll;

	static TextListboxWidget listBox;

	private ref WidgetStore widgetStore;

	void ObjectInfoMenu()
	{

	}

	void ~ObjectInfoMenu()
	{
		GetGame().GetCallQueue( CALL_CATEGORY_GUI ).Remove( Update );
	}

	static void UpdateObjectList()
	{
		listBox.ClearItems();

		auto objects = ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor)).m_Objects;

		foreach( Object obj : objects )
		{
			listBox.AddItem(obj.GetType(), obj, 0); // store object ref into list?
		}
	}

	Object GetSelectedRowObject()
	{
		if ( listBox.GetSelectedRow() != -1 )
		{
			Object result;
			listBox.GetItemData( listBox.GetSelectedRow(), 0, result );
			return result;
		}

		return NULL;
	}

	override void Init( )
	{

		widgetStore = new WidgetStore( layoutRoot );

		listBox = widgetStore.GetListboxWidget("object_editor_info_list");

		infoPosX = EditBoxWidget.Cast(layoutRoot.FindAnyWidget( "object_info_pos_x" ));
		infoPosY = EditBoxWidget.Cast(layoutRoot.FindAnyWidget( "object_info_pos_y" ));
		infoPosZ = EditBoxWidget.Cast(layoutRoot.FindAnyWidget( "object_info_pos_z" ));
		infoPosYaw = EditBoxWidget.Cast(layoutRoot.FindAnyWidget( "object_info_yaw_input" ));
		infoPosPitch = EditBoxWidget.Cast(layoutRoot.FindAnyWidget( "object_info_pitch_input" ));
		infoPosRoll = EditBoxWidget.Cast(layoutRoot.FindAnyWidget( "object_info_roll_input" ));
	}

	override bool OnItemSelected( Widget w, int x, int y, int row, int column, int oldRow, int oldColumn )
	{
		if ( w.GetName() == "object_editor_info_list")
		{
			Object selected = GetSelectedRowObject();
			if ( selected )
			{
				ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor)).SelectObject( selected );
			}
		}

		return false;
	}

	override bool OnClick( Widget w, int x, int y, int button )
	{
		if ( w.GetName() == "object_editor_info_export")
		{
			ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor)).ExportScene();
		}
		if ( w.GetName() == "object_editor_info_save")
		{
			ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor)).SaveScene();
		}
		if ( w.GetName() == "object_editor_info_clear")
		{
			auto objects = ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor)).m_Objects;

			foreach( Object obj : objects )
			{
				GetGame().ObjectDelete( obj );
			}
			objects.Clear();
			UpdateObjectList();
		}
		if ( w.GetName() == "object_editor_info_dumplods" )
		{
			string toCopy = "";
			array<LOD> lods = new array<LOD>;
			Object object = ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor)).m_SelectedObject;
			object.GetLODS(lods);

			foreach( LOD lod : lods )
			{
				toCopy = toCopy + object.GetLODName( lod ) + "\n";
				array<Selection> selections = new array<Selection>;
				lod.GetSelections( selections );
				foreach ( Selection selection : selections )
				{
					toCopy = toCopy + " " + selection.GetName() + "\n";
				}
			}
			GetGame().CopyToClipboard(toCopy);
			COM_Message("Dumped LODs to clipboard");
		}
		if ( w.GetName() == "object_editor_info_load")
		{
			objects = ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor)).m_Objects;

			foreach( Object obj2 : objects )
			{
				GetGame().ObjectDelete( obj2 );
			}
			objects.Clear();
			ObjectEditor.Cast(COM_GetModuleManager().GetModule(ObjectEditor)).LoadScene();
			UpdateObjectList();
		}
		return false;
	}

	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		if ( !GetSelectedObject() )
		{
			return false;
		}
		vector orientation = GetSelectedObject().GetOrientation();
		vector position = GetSelectedObject().GetPosition();

		bool up = wheel < 0;
		int value = 1;

		if ( up ) value = -1;

		if ( w == infoPosYaw )
		{
			orientation[0] = orientation[0] + value;

			if( Math.AbsFloat( orientation[0] ) < 0.001 )
			{
			    orientation[0] = 0;
			}

			GetSelectedObject().SetOrientation( orientation );
			infoPosYaw.SetText( orientation[0].ToString() );
		}
		if ( w == infoPosPitch )
		{
			if ( orientation[0] > 0 ) // seemless pitch change
			{
				value = -value;
			}

			orientation[1] = orientation[1] + value;

            if( Math.AbsFloat( orientation[1] ) < 0.001 )
            {
                orientation[1] = 0;
            }

			GetSelectedObject().SetOrientation( orientation );
			infoPosPitch.SetText( orientation[1].ToString() );

		}
		if ( w == infoPosRoll )
		{
			orientation[2] = orientation[2] + value;

            if( Math.AbsFloat( orientation[2] ) < 0.001 )
            {
                orientation[2] = 0;
            }

			GetSelectedObject().SetOrientation( orientation );
			infoPosRoll.SetText( orientation[2].ToString() );
		}
		if ( w == infoPosY )
		{
			position[1] = position[1] + value * 0.05;
			GetSelectedObject().SetPosition( position );
			COM_ForceTargetCollisionUpdate( GetSelectedObject() );
			infoPosY.SetText( position[1].ToString() );
		}
		if ( w == infoPosX )
		{
			position[0] = position[0] + (value * 0.05);
			GetSelectedObject().SetPosition( position );
			COM_ForceTargetCollisionUpdate( GetSelectedObject() );
			infoPosX.SetText( position[0].ToString() );
		}
		if ( w == infoPosZ )
		{
			position[2] = position[2] + value * 0.05;
			GetSelectedObject().SetPosition( position );
			COM_ForceTargetCollisionUpdate( GetSelectedObject() );
			infoPosZ.SetText( position[2].ToString() );
		}
		return false;
	}

	override bool OnChange( Widget w, int x, int y, bool finished ) // finished = press enter
	{
		if ( !w.IsInherited( EditBoxWidget ))
		{
			return false;
		}

		vector pos = GetSelectedObject().GetPosition();
		vector orientation = GetSelectedObject().GetOrientation();

		EditBoxWidget editWidget = EditBoxWidget.Cast(w);
		string text = editWidget.GetText();

		float value = text.ToFloat();

//		Print("float value = text.ToFloat() = " + value);

		if ( editWidget == infoPosYaw )
		{
			orientation[0] = value;
		}
		if ( editWidget == infoPosPitch )
		{
			orientation[1] = value;
		}
		if ( editWidget == infoPosRoll )
		{
			orientation[2] = value;
		}
		if ( editWidget == infoPosX )
		{
			pos[0] = value;
		}
		if ( editWidget == infoPosY )
		{
			pos[1] = value;
		}
		if ( editWidget == infoPosZ )
		{
			pos[2] = value;
		}
		GetSelectedObject().SetPosition( pos );
		GetSelectedObject().SetOrientation( orientation );

/*
		bool check = false; //?????????????

        if ( editWidget == infoPosPitch )
        {
        	for(int i = 0; i < text.Length(); i++ )
        	{
        		string token;
        		string character = text.Get(i);
        		int result = character.ParseStringEx(token);

        		if ( result == 4 || text.Get(i) == "." )
        		{
        			newText = newText + token;
        			check = true; // lol wtf why this crashing. come back to this
        		}
        	}

        	if ( check ) // idk why this bool is causing unknown module crash
        	{
        		orientation[1] = newText.ToFloat();
        		GetSelectedObject().SetOrientation(orientation);
        	}
        	else
        	{
        		editWidget.SetText( newText );
        	}

            return false;
        }

        */

        return false;
    }

    override bool OnMouseEnter(Widget w, int x, int y)
	{
		return false;
	}
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		SetFocus( layoutRoot );
		return false;
	}

    override void OnShow()
    {
    	UpdateObjectList();
    	GetGame().GetCallQueue( CALL_CATEGORY_GUI ).CallLater( Update, 100, true );
    }

    override void OnHide()
    {
    	//GetGame().GetUpdateQueue( CALL_CATEGORY_GUI ).Remove( Update );
    }

	override void Update()
	{
		Object selectedObject = GetSelectedObject();
		string text = "No object Selected";
		TStringArray paths = new TStringArray;

		bool selected = false;

		Widget focusedWidget = GetFocus();

		if ( selectedObject )
		{
			infoPosX.ClearFlags( WidgetFlags.IGNOREPOINTER );
			infoPosY.ClearFlags( WidgetFlags.IGNOREPOINTER );
			infoPosZ.ClearFlags( WidgetFlags.IGNOREPOINTER );
			infoPosYaw.ClearFlags( WidgetFlags.IGNOREPOINTER );
			infoPosPitch.ClearFlags( WidgetFlags.IGNOREPOINTER );
			infoPosRoll.ClearFlags( WidgetFlags.IGNOREPOINTER );

			text = selectedObject.GetType();
		} else
		{
			infoPosX.SetFlags( WidgetFlags.IGNOREPOINTER );
			infoPosY.SetFlags( WidgetFlags.IGNOREPOINTER );
			infoPosZ.SetFlags( WidgetFlags.IGNOREPOINTER );
			infoPosYaw.SetFlags( WidgetFlags.IGNOREPOINTER );
			infoPosPitch.SetFlags( WidgetFlags.IGNOREPOINTER );
			infoPosRoll.SetFlags( WidgetFlags.IGNOREPOINTER );
		}

		TextWidget selectedObjectWidget = TextWidget.Cast(layoutRoot.FindAnyWidget( "object_editor_info_select_input" ));
		selectedObjectWidget.SetText( text );
		// SetFlags(int flags, bool immedUpdate = true);
	}

	Object GetSelectedObject()
	{
		return ObjectEditor.Cast(COM_GetModuleManager().GetModule( ObjectEditor )).m_SelectedObject;
	}
}

// ===== core/modules/ComEditor/gui/OverrideVerticalSpacer.c =====
// -----------------------------------------------------------
class OverrideVerticalSpacer : SpacerBase
{
	reference int border;
	reference int gap;
	reference int maxheight;

	override protected void UpdateChild(Widget child, float w, float h, int index)
	{
		float itemWidth = w - (2 * border);
		float itemHeight = (h - (border * 2) - ((m_count - 1) * gap)) / m_count;
	
		if ( itemHeight > maxheight ) 
		{
			itemHeight = maxheight;
		}

		child.SetPos(border, border + ((itemHeight + gap) * index));
		child.SetSize(itemWidth, itemHeight);
	}
}
// ===== core/modules/ComEditor/scene/Scene.c =====
/**
\brief Represents active loaded scenes in the game	

*/
class Scene 
{
	ref array<Object> s_Objects = new array<Object>; // All active spawned objects
	ref SceneData sceneData;

	void Scene( SceneData data ) 
	{
		sceneData = data;
	}

	void ~Scene() 
	{

	}
}
// ===== core/modules/ComEditor/scene/SceneManager.c =====
string SCENE_DIR = "$profile:SceneManager";
string SCENE_ACTIVE = SCENE_DIR + "\\ActiveScenes";
string SCENE_DATA = SCENE_DIR + "\\Scenes";

class SceneManager
{
	ref map< SceneData, ref array< Scene >> sm_Scenes = new map< SceneData, ref array< Scene>>;

	void SceneManager() 
	{
		MakeDirectory( SCENE_DATA );

		Init();
	}

	void ~SceneManager() 
	{

	}

	void Init() 
	{
		// load scenes
	}

	void LoadScenes() 
	{
		FileAttr fileAttr;
		string fileName;

		FindFileHandle fileHandle = FindFile( SCENE_DIR + "\\" + "*.json", fileName, fileAttr, FindFileFlags.ALL );
		if ( fileName != "" ) 
		{
			while (FindNextFile( fileHandle, fileName, fileAttr )) 
			{
				sm_Scenes.Insert( LoadSceneData( fileName ), new array< Scene > );
			}
		}
	}

	SceneData LoadSceneData( string fileName ) 
	{
		SceneData data;
		JsonFileLoader<SceneData>.JsonLoadFile( SCENE_DIR + "\\" + fileName + ".json", data );
		return data;
	}
}
// ===== core/modules/ComEditor/scene/SceneInfo.c =====
/**
\brief Stores loaded scene data information from json file. ie object positions, loot positions, duration etc

*/
class SceneInfo
{

	//ref map< ObjectData, array< LootSpot >>  m_objects = new map< ObjectData, array< LootSpot >>;
	string name;
	ref array< ObjectData > m_objects = new array< ObjectData >;
	ref array< LootSpot > m_lootSpots = new array< LootSpot >;

	bool persistent = false;
	int duration = 0; // in seconds

	void SceneInfo( string sceneName ) 
	{
		name = sceneName;
	}

	void ~SceneInfo() 
	{

	}

	string GetName() 
	{
		return name;
	}

/*
	ObjectData GetObjectData( vector position ) 
	{
		for(int i = 0; i<m_objects.Count();i++ ) 
		{
			ObjectData data = m_objects.GetKey( i );
			if ( data.GetPosition() == position ) 
			{
				return data;
			}
		}
		return NULL;
	}
*/
	void AddObject( Object object, vector pos ) 
	{
		m_objects.Insert( new ObjectData( object.GetType(), pos, object.GetOrientation() ));
	}

	void AddLootSpot( vector pos, LootType type ) 
	{
		m_lootSpots.Insert( new LootSpot( pos, type ) );
	}

/* Do not use below */

/*
	void AddObject( Object object ) 
	{
		AddObject( object.GetType(), object.GetPosition(), object.GetOrientation() );
	}

	void AddObject( string classname, vector position, vector orientation ) 
	{
		m_objects.Insert( new ObjectData( classname, position, orientation ), new array< LootSpot > );
	}
*/
}

// create scene (set origin point to player position)
// create object containers with loot spots
// add object container to scene
// ===== core/modules/ComEditor/scene/ObjectData.c =====
class ObjectData 
{
	string classname;
	vector position;
	vector orientation;

	void ObjectData( string name, vector pos, vector ori )
	{
		classname = name;
		position = pos;
		orientation = ori;
	}

	string GetClassName() 
	{
		return classname;
	}

	vector GetPosition() 
	{
		return position;
	}

	vector GetOrientation() 
	{
		return orientation;
	}
}
// ===== core/modules/ComEditor/scene/LootSpot.c =====
class LootSpot
{
	LootType type;
	vector position;

	void LootSpot( vector pos, LootType lootType ) 
	{
		position = pos;
		type = lootType;
	}

	vector GetPosition() 
	{
		return position;
	}

	LootType GetType() 
	{
		return type;
	}
}

enum LootType 
{
	INDUSTRIAL, // tools
	VEHICLE, // vehicle parts
	MEDICAL, // medical items
	CIVILIAN, // civ items - axe, can opener, knife, etc
	FOOD,
	MILITARY
}
// ===== core/modules/ComEditor/scene/SceneSaveST.c =====
class SceneSaveST 
{
	// temporary class
	string name;
	ref array<ref Param3<string, vector, vector>> m_SceneObjects = new array< ref Param3<string, vector, vector>>;
} 
#endif

#ifndef COM_MODULES_OLDLOADING
void RegisterModule()
{
    COM_GetModuleManager().RegisterModule( new ObjectEditor );
}
#endif
// ===== core/modules/ComKeybinds/module.c =====
#ifndef COM_MODULES_OLDLOADING
#endif
/*
    Define used for optional compilations
*/
#define MODULE_COM_KEYBINDS

/*
    Include of all .c files that belong to this module
*/
#ifdef COM_MODULES_OLDLOADING
// ===== core/modules/ComKeybinds/COMKeyBinds.c =====
/*
	Manages all keybinds for COM
*/
class COMKeyBinds extends Module 
{
	bool m_IsHudVisible = true;
	protected int m_nAutoWalkMode = 0;

	void COMKeyBinds()
	{
	}

	void ~COMKeyBinds()
	{
	}
	
	override void Init() 
	{
		super.Init();
	}
	
	override void onUpdate( float timeslice )
	{
	    UpdateAutoWalk();

	    UpdateGodMode();
	}
	
	override void RegisterKeyMouseBindings() 
	{
		KeyMouseBinding toggleCursor    = new KeyMouseBinding( GetModuleType() , "ToggleCursor"  , "Toggles the cursor."   , true     );
		KeyMouseBinding toggleCOMEditor = new KeyMouseBinding( GetModuleType() , "ShowCOMEditor" ,  "Opens the COM Editor."           );
		KeyMouseBinding teleport	    = new KeyMouseBinding( GetModuleType() , "TeleportCursor",  "Teleport to cursor position."    );
		KeyMouseBinding reload          = new KeyMouseBinding( GetModuleType() , "Reload"        ,  "Instantly reloads mag."		  );
        KeyMouseBinding spawnZ          = new KeyMouseBinding( GetModuleType() , "SpawnZ"        ,  "Spawns infected."                );
        KeyMouseBinding hideHud         = new KeyMouseBinding( GetModuleType() , "HideHud"       ,  "Hides ui completely."            );
        KeyMouseBinding printPlayer     = new KeyMouseBinding( GetModuleType() , "PrintPlayer"   ,  "Print current player position."  );
        KeyMouseBinding autoRun         = new KeyMouseBinding( GetModuleType() , "AutoRun"       ,  "Toggle autorun."                 );
//        KeyMouseBinding keyFrame        = new KeyMouseBinding( GetModuleType() , "OpenKeyframe"  ,  "Toggle dayz dev cinematic tool." );
        KeyMouseBinding closeMenu       = new KeyMouseBinding( GetModuleType() , "CloseOpenMenu" ,  "Close the menu on esc.", true    );

		toggleCursor   .AddBinding( "kU" );
		toggleCOMEditor.AddBinding( "kY" );
		teleport       .AddBinding( "kEnd" );
		reload         .AddBinding( "kR" );
        spawnZ         .AddBinding( "kO" );
        hideHud        .AddBinding( "kHome" );
        printPlayer    .AddBinding( "kP" );
        autoRun        .AddBinding( "kX" );
//        keyFrame       .AddBinding( "kPrior" );
        closeMenu      .AddBinding( "kEscape" );

		RegisterKeyMouseBinding( toggleCursor );
		RegisterKeyMouseBinding( toggleCOMEditor );
		RegisterKeyMouseBinding( teleport );
		RegisterKeyMouseBinding( reload );
        RegisterKeyMouseBinding( spawnZ );
        RegisterKeyMouseBinding( hideHud );
        RegisterKeyMouseBinding( printPlayer );
        RegisterKeyMouseBinding( autoRun );
//        RegisterKeyMouseBinding( keyFrame );
        RegisterKeyMouseBinding( closeMenu );
	}

//    void OpenKeyframe()
//    {
//        GetGame().GetUIManager().ShowScriptedMenu( COM_GetMission().CreateScriptedMenu(MENU_CAMERA_TOOLS) , NULL );
//    }

    void COM_ToggleCursor()
    {
        if ( GetGame().GetInput().HasGameFocus( INPUT_DEVICE_MOUSE ) )
        {
            GetGame().GetInput().ChangeGameFocus( 1 );
            GetGame().GetUIManager().ShowUICursor( true );
            FreeDebugCamera.GetInstance().SetFreezed(true);
        }
        else
        {
            GetGame().GetUIManager().ShowUICursor( false );
            GetGame().GetInput().ResetGameFocus();
            FreeDebugCamera.GetInstance().SetFreezed(false);
        }
         COM_Message(GetDayZGame().GetMissionFolderPath());
    }

    void ToggleCursor()
    {
        if ( GetGame().GetInput().HasGameFocus( INPUT_DEVICE_MOUSE ) )
        {
            GetGame().GetInput().ChangeGameFocus( 1 );
            GetGame().GetUIManager().ShowUICursor( true );
        }
        else
        {
            GetGame().GetUIManager().ShowUICursor( false );
            GetGame().GetInput().ResetGameFocus();
        }
    }

    void CloseOpenMenu()
    {
        if( GetGame().GetUIManager().GetMenu() && ( GetGame().GetUIManager().GetMenu().GetID() == 133742 ) )
        {
            GetGame().GetUIManager().Back();
        }
    }

    void ShowCOMEditor()
    {
        UIMenuPanel currentMenu = GetGame().GetUIManager().GetMenu();
        if ( currentMenu && currentMenu.GetID() == EditorMenu.MENU_ID )
        {
            GetGame().GetUIManager().Back();
            return;
        }

        GetGame().GetUIManager().EnterScriptedMenu(EditorMenu.MENU_ID, NULL);
    }

    void TeleportCursor()
    {
        Print( "COMKeyBinds::TeleportCursor()" );

//		if ( CameraTool.Cast( m_Mission.GetModule( CameraTool ) ).IsUsingCamera() ) //Todo renable after module manager is done
//		{
//			COM_GetPB().MessageStatus( "You can not teleport while you are inside the freecam!" );
//			return;
//		}
		
        vector hitPos = COM_GetCursorPos();

        float distance = vector.Distance( COM_GetPB().GetPosition(), hitPos );

        if ( distance < 5000 )
        {
            EntityAI oVehicle = COM_GetPB().GetDrivingVehicle();

            if( oVehicle )
            {
                COM_GetPB().MessageStatus( "Get out of the vehicle first!" );
            }
            else
            {
                COM_GetPB().SetPosition( hitPos );
                COM_GetPB().MessageStatus( "Teleported!" );
            }
        }
        else
        {
            COM_GetPB().MessageStatus( "Distance for teleportation is too far!" );
        }
    }

    void Reload()
    {
        auto wpn = COM_GetPB().GetHumanInventory().GetEntityInHands();

        if( wpn )
        {
            auto magAtt = wpn.GetAttachmentByConfigTypeName( "DefaultMagazine" );

            if( magAtt )
            {
                auto mag = Magazine.Cast( magAtt );

                if( mag && mag.IsMagazine() )
                {
                    mag.LocalSetAmmoMax();
                }
            }

            auto suppAtt = wpn.GetAttachmentByConfigTypeName( "ItemSuppressor" );

            if( suppAtt )
            {
                auto supp = suppAtt;

                if( supp )
                {
                    supp.SetHealth( supp.GetMaxHealth( "", "" ) );
                }
            }
        }
    }

    void SpawnZ() 
    {
        if( COM_CTRL() )
        {
            GetGame().CreateObject( "Animal_CanisLupus_Grey", COM_GetCursorPos(), false, true );
        }
        else if( COM_SHIFT() )
        {
            GetGame().CreateObject( COM_GetRandomChildFromBaseClass( "cfgVehicles", "AnimalBase" ), COM_GetCursorPos(), false, true );
        }
        else
        {
            GetGame().CreateObject( COM_GetRandomChildFromBaseClass( "cfgVehicles", "ZombieBase", 2 ), COM_GetCursorPos(), false, true );
        }
    }

    void HideHud() 
    {
        Widget hudWidget = IngameHud.Cast(COM_GetClientMission().GetHud()).GetHudPanelWidget();
        hudWidget.Show(!hudWidget.IsVisible());
    }

    void PrintPlayer()
    {
        Print( "Position:" + COM_GetPB().GetPosition().ToString() );
        Print( "Orientation" + COM_GetPB().GetOrientation().ToString() );
        COM_Message( "POS X:" + COM_GetPB().GetPosition()[0] + " Y:" + COM_GetPB().GetPosition()[2] + " Z:" + COM_GetPB().GetPosition()[1] );
        COM_Message( "Player position and orientation vector were written to the game logs too." );
    }

    void AutoRun()
    {
        if( m_nAutoWalkMode && !COM_SHIFT() && !COM_CTRL() )
        {
            m_nAutoWalkMode = 0;
            COM_GetPB().GetInputController().OverrideMovementSpeed( false, 0 );
            COM_GetPB().GetInputController().OverrideMovementAngle( false, 0 );
        }
        else
        {
            if( COM_SHIFT() )
            {
                m_nAutoWalkMode = 3;
            }
            else if( COM_CTRL() )
            {
                m_nAutoWalkMode = 1; //CTRL == slow mode
            }
            else
            {
                m_nAutoWalkMode = 2;
            }
        }
    }

    void UpdateAutoWalk()
    {
        if( m_nAutoWalkMode > 0 )
        {
            if( m_nAutoWalkMode == 1 )
            {
                COM_GetPB().GetInputController().OverrideMovementSpeed( true, 1 );
            }
            else if( ( COM_GetPB().GetInputController().LimitsIsSprintDisabled() ) || ( m_nAutoWalkMode == 2 ) )
            {
                COM_GetPB().GetInputController().OverrideMovementSpeed( true, 2 );
            }
            else if( m_nAutoWalkMode == 3 )
            {
                COM_GetPB().GetInputController().OverrideMovementSpeed( true, 3 );
            }

            COM_GetPB().GetInputController().OverrideMovementAngle( true, 1 );
        }
    }

    void UpdateGodMode()
    {
        // just putting this here for now
        if ( m_COM_GodMode ) // located in staticfunctions
        {
            COM_GetPB().SetHealth( COM_GetPB().GetMaxHealth( "", "" ) );
            COM_GetPB().SetHealth( "","Blood", COM_GetPB().GetMaxHealth( "", "Blood" ) );
            COM_GetPB().SetHealth( "","Shock", COM_GetPB().GetMaxHealth( "", "Shock" ) );

            //GetPlayer().GetStaminaHandler().SyncStamina(1000, 1000);
            COM_GetPB().GetStatStamina().Set(COM_GetPB().GetStaminaHandler().GetStaminaCap());
            COM_GetPB().GetStatEnergy().Set(1000);
            COM_GetPB().GetStatWater().Set(1000);
//            COM_GetPB().GetStatStomachVolume().Set(300);
//            COM_GetPB().GetStatStomachWater().Set(300);
//            COM_GetPB().GetStatStomachEnergy().Set(300);
            COM_GetPB().GetStatHeatComfort().Set(0);

            EntityAI oWeapon = COM_GetPB().GetHumanInventory().GetEntityInHands();

            if( oWeapon )
            {
                Magazine oMag = Magazine.Cast(oWeapon.GetAttachmentByConfigTypeName( "DefaultMagazine" ));

                if( oMag && oMag.IsMagazine() )
                {
                    oMag.LocalSetAmmoMax();
                }

                Object oSupressor = ( Object ) oWeapon.GetAttachmentByConfigTypeName( "SuppressorBase" );

                if( oSupressor )
                {
                    oSupressor.SetHealth( oSupressor.GetMaxHealth( "", "" ) );
                }
            }
        }
    }
}
#endif

#ifndef COM_MODULES_OLDLOADING
void RegisterModule()
{
    COM_GetModuleManager().RegisterModule( new COMKeyBinds );
}
#endif
// ===== core/modules/ComMenu/module.c =====
/*
    Define used for optional compilations
*/
#define MODULE_COM_MENU

/*
    Include of all .c files that belong to this module
*/
// ===== core/modules/ComMenu/gui/WelcomeMenu.c =====
class WelcomeMenu extends UIScriptedMenu
{
    MultilineTextWidget m_InfoBox;
    ButtonWidget m_BtnOk;

	void WelcomeMenu()
	{
	}	
	
	void ~WelcomeMenu()
	{
	}

    override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets( "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\ComMenu\\gui\\layouts\\WelcomeMenu.layout" );

        m_InfoBox = MultilineTextWidget.Cast(TextWidget.Cast( layoutRoot.FindAnyWidget( "infotext" ) ));
        m_BtnOk = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "btn_ok" ) );

		string infotext;
		
		infotext = "Enjoy the DayZ Community Sandbox Mode.";
		infotext += "\n \n \nThe current game build is still ALPHA!";
		infotext += "\nCrashes and bugs can occur.";
		infotext += "\nNot all features are implemented or working yet!";
		infotext += "\nProvide feedback on the game via feedback.dayz.com";
		infotext += "\n \n \nCreated and Managed by Arkensor, DannyDog and Jacob_Mango.";
		infotext += "\n \n \nContributions from:";
		infotext += "\n DuhOneZ, Watchman, n8m4re, wriley and gallexme.";

        m_InfoBox.SetText( infotext );

        return layoutRoot;
    }

    override void OnShow()
    {
        super.OnShow();
    }

    override void OnHide()
    {
        super.OnHide();
    }

	override bool OnClick( Widget w, int x, int y, int button )
	{
		super.OnClick(w, x, y, button);

		if ( w == m_BtnOk )
		{
			Close();

			return true;
		}

		return false;
	}

	override bool OnKeyPress( Widget w, int x, int y, int key )
	{
        if( key == KeyCode.KC_ESCAPE )
        {
			Close();
        }

		return false;
	}
}

//#include "$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\Persistency\\module.c"
// ===== core/modules/DebugMonitor/module.c =====
#ifndef COM_MODULES_OLDLOADING
#endif
/*
    Define used for optional compilations
*/
#define MODULE_DEBUG_MONITOR

/*
    Include of all .c files that belong to this module
*/
#ifdef COM_MODULES_OLDLOADING
// ===== core/modules/DebugMonitor/CustomDebugMonitor.c =====
/*
	Manages all keybinds for COM
*/
class CustomDebugMonitor extends Module
{
	protected bool m_IsVisible = false;
	protected ref ArkDebugMonitor m_debugMonitor;

	void CustomDebugMonitor()
	{
	    m_debugMonitor = new ArkDebugMonitor;
	}

	void ~CustomDebugMonitor()
	{
	}
	
	override void Init() 
	{
		super.Init();

		m_debugMonitor.Init();
		m_debugMonitor.Hide();
	}
	
	override void onUpdate( float timeslice )
	{
        if( m_IsVisible )
        {
            PlayerBase player = PlayerBase.Cast( COM_GetPB() );

            if ( player )
            {
                DebugMonitorValues values = player.GetDebugMonitorValues();

                if ( values )
                {
                    auto health = player.GetHealth("","Health");
                    auto blood = player.GetHealth("","Blood");
                    auto lastdamage = values.GetLastDamage();

                    if( lastdamage == "" )
                    {
                        lastdamage = "Unknown";
                    }

                    auto position = player.GetPosition();
                    auto orientation = GetGame().GetCurrentCameraDirection().VectorToAngles();

                    m_debugMonitor.SetHealth( health );
                    m_debugMonitor.SetBlood( blood );
                    m_debugMonitor.SetLastDamage( lastdamage );
                    m_debugMonitor.SetPosition( position );
                    m_debugMonitor.SetOrientation( orientation );

                    int year;
                    int month;
                    int day;
                    int hour;
                    int minute;
                    int second;

                    GetYearMonthDay(year, month, day);
                    GetHourMinuteSecond(hour, minute, second);

                    string date = day.ToStringLen(2) + "-" + month.ToStringLen(2) + "-" + year.ToStringLen(2) + " " + hour.ToStringLen(2) + ":" + minute.ToStringLen(2) + ":" + second.ToStringLen(2);

                    m_debugMonitor.SetDateTime( date );
                }
            }
        }
	}
	
	override void RegisterKeyMouseBindings() 
	{
		auto oToggleMonitor = new KeyMouseBinding( GetModuleType(), "ToggleMonitor", "Toggles the debug monitor." );

		oToggleMonitor.AddBinding( "kB" );

		RegisterKeyMouseBinding( oToggleMonitor );
	}

    void ToggleMonitor()
    {
        m_IsVisible = !m_IsVisible;

        if( m_IsVisible )
        {
            m_debugMonitor.Show();
        }
        else
        {
            m_debugMonitor.Hide();
        }
    }
}
// ===== core/modules/DebugMonitor/gui/DebugMonitor.c =====
class ArkDebugMonitor
{
	private Widget m_WidgetRoot;
	private TextWidget m_pWindowLabelText;
	private TextListboxWidget m_pNamesListboxWidget;
	private TextListboxWidget m_pValuesListboxWidget;
	private MultilineTextWidget m_pModifiersMultiTextWidget;

	void ArkDebugMonitor()
	{
		m_WidgetRoot = GetGame().GetWorkspace().CreateWidgets("$CurrentDir:missions\\DayZCommunityOfflineMode.Enoch\\core\\modules\\DebugMonitor\\gui\\layouts\\day_z_debug_monitor.layout");
		m_WidgetRoot.Show(false);

		m_pWindowLabelText = TextWidget.Cast( m_WidgetRoot.FindAnyWidget("WindowLabel") );
		m_pNamesListboxWidget = TextListboxWidget.Cast( m_WidgetRoot.FindAnyWidget("NamesListboxWidget") );
		m_pValuesListboxWidget = TextListboxWidget.Cast( m_WidgetRoot.FindAnyWidget("ValuesListboxWidget") );
		m_pModifiersMultiTextWidget = MultilineTextWidget.Cast( m_WidgetRoot.FindAnyWidget("ModifiersMultilineTextWidget") );
	}

	void Init()
	{
		m_pNamesListboxWidget.AddItem("HEALTH:", NULL, 0);
		m_pValuesListboxWidget.AddItem("", NULL, 0);

		m_pNamesListboxWidget.AddItem("BLOOD:", NULL, 0);
		m_pValuesListboxWidget.AddItem("", NULL, 0);

		/*m_pNamesListboxWidget.AddItem("BODY TEMP:", NULL, 0);
		m_pValuesListboxWidget.AddItem("", NULL, 0);*/

		m_pNamesListboxWidget.AddItem("LAST DAMAGE:", NULL, 0);
		m_pValuesListboxWidget.AddItem("", NULL, 0);

		m_pNamesListboxWidget.AddItem("POSITION:", NULL, 0);
		m_pValuesListboxWidget.AddItem("", NULL, 0);

		m_pNamesListboxWidget.AddItem("ORIENTATION:", NULL, 0);
		m_pValuesListboxWidget.AddItem("", NULL, 0);

		m_pNamesListboxWidget.AddItem("DATETIME:", NULL, 0);
		m_pValuesListboxWidget.AddItem("", NULL, 0);

        string version;
        GetGame().GetVersion(version);

		m_pNamesListboxWidget.AddItem("VERSION:", NULL, 0);
		m_pValuesListboxWidget.AddItem( " " + version, NULL, 0);

		/*m_pNamesListboxWidget.AddItem("MODIFIERS:", NULL, 0);
		m_pValuesListboxWidget.AddItem("", NULL, 0);*/

		//m_DebugFloatsListWidget.AddItem(key,NULL,0);
		//m_DebugFloatsListWidget.SetItem(i,value.ToString(),NULL,1);

		m_WidgetRoot.Show(true);
	}

	void SetHealth(float value)
	{
		string health = string.Format(" %1", value.ToString());
		m_pValuesListboxWidget.SetItem(0, health, NULL, 0);
	}

	void SetBlood(float value)
	{
		string blood = string.Format(" %1", value.ToString());
		m_pValuesListboxWidget.SetItem(1, blood, NULL, 0);
	}

	void SetLastDamage(string value)
	{
		string lastDamage = string.Format(" %1", value);
		m_pValuesListboxWidget.SetItem(2, lastDamage, NULL, 0);
	}

	void SetPosition(vector value)
	{
		string position = string.Format(" %1 %2 %3", value[0].ToString(), value[1].ToString(), value[2].ToString());
		m_pValuesListboxWidget.SetItem(3, position, NULL, 0);
	}

	void SetOrientation(vector value)
	{
		string orientation = string.Format(" %1 %2 %3", value[0].ToString(), value[1].ToString(), value[2].ToString());
		m_pValuesListboxWidget.SetItem(4, orientation, NULL, 0);
	}

	void SetDateTime(string value)
	{
		string orientation = string.Format(" %1", value);
		m_pValuesListboxWidget.SetItem(5, orientation, NULL, 0);
	}

	void Hide()
	{
		m_WidgetRoot.Show(false);
	}

	void Show()
	{
		m_WidgetRoot.Show(true);
	}
};
#endif

#ifndef COM_MODULES_OLDLOADING
void RegisterModule()
{
    COM_GetModuleManager().RegisterModule( new CustomDebugMonitor );
}
#endif
// ===== core/modules/BarrelCrosshair/module.c =====
/*
    Define used for optional compilations
*/
#define MODULE_BARREL_CROSSHAIR

/*
    Include of all .c files that belong to this module
*/

#ifdef COM_MODULES_OLDLOADING
// ===== core/modules/BarrelCrosshair/gui/BarrelCrosshair.c =====
class BarrelCrosshair extends ScriptedWidgetEventHandler 
{
	protected Widget m_root;
	
	private Weapon		m_Weapon;
	private vector		m_Position;
	private PlayerBase  m_Player;
	
	float distance = 0;
	
	void BarrelCrosshair()
	{
		m_Player = NULL;
		m_Weapon = NULL;
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(this.Update);
	}
	
	void ~BarrelCrosshair()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(this.Update);
	}
	
	void OnWidgetScriptInit(Widget w)
	{
		m_root = w;
		m_root.SetHandler(this);
		m_root.Update();
	}
	
	private void Update()
	{
		if(!m_Player) Class.CastTo(m_Player, COM_GetPB());
		
		if ( COM_bc_Visible ) 
		{
			if( m_Player && m_Player.IsPlayerSelected() && !m_Player.IsInIronsights() && !GetGame().IsInventoryOpen() )
			{
				float sx, sy;

				GetCrosshairPosition();
				vector screenSpace = GetGame().GetScreenPos(m_Position);

				m_root.GetSize(sx, sy);
				screenSpace[0] = screenSpace[0] - sx/2.0;
				screenSpace[1] = screenSpace[1] - sy/2.0;

				/*
				float size = 0.005 - distance/100000.0;
				if ( size < 0.002 ) size = 0.002;
				m_root.SetSize(size, size);
				*/
				
				m_root.SetPos(screenSpace[0], screenSpace[1]);
				// m_root.SetAlpha(alpha);
				
				m_root.Show(true);
				
				return;
			}
		}
		
		if ( m_root.IsVisible() ) 
		{
			m_root.Show(false);
			m_Position = vector.Zero;
		}
	}
	
	Weapon COM_GetWeaponInHands()
	{
		Weapon weapon_in_hands;
		PlayerBase player = PlayerBase.Cast(COM_GetPB());
		if( player && player.GetItemInHands() ) Class.CastTo(weapon_in_hands,  player.GetItemInHands());
		
		return weapon_in_hands;
	}
	
	private void GetCrosshairPosition()
	{
		ItemBase itemInHands;
		itemInHands = m_Player.GetItemInHands();
		if( itemInHands && itemInHands.IsWeapon() )
		{
			if( Class.CastTo(m_Weapon, itemInHands) )
			{
				vector usti_hlavne_position = GetGame().ObjectGetSelectionPosition( m_Weapon, "usti hlavne" );
				vector konec_hlavne_position = GetGame().ObjectGetSelectionPosition( m_Weapon, "konec hlavne" );
				
				vector end_point = m_Weapon.ModelToWorld(usti_hlavne_position);
				vector begin_point = m_Weapon.ModelToWorld(konec_hlavne_position);
				
				vector contact_dir;
				int contact_component;
				
				vector aim_point = end_point - begin_point;
				aim_point = aim_point.Normalized() * 9999;
				aim_point = aim_point + end_point;
				
				if (DayZPhysics.RaycastRV(begin_point, aim_point, m_Position, contact_dir, contact_component, null, null, null, false, false, ObjIntersectFire)) 
				{
					distance = vector.Distance(begin_point, m_Position);
				}
			}
		}
	}
}
#endif


#endif

class KeyMouseActionType
{
    static int PRESS = 1;
    static int RELEASE = 2;
    static int HOLD = 4;
    static int DOUBLECLICK = 8;
    static int VALUE = 16;
}

class ModuleManager
{
    protected ref array< ref Module > m_Modules;

    void ModuleManager()
    {
        RegisterModules();
    }

    void ~ModuleManager()
    {
        m_Modules.Clear();

        delete m_Modules;
    }

    void RegisterModule( Module module )
    {
        m_Modules.Insert( module );
    }

    void RegisterModules()
    {
        m_Modules = new array< ref Module >;

        #ifdef MODULE_COM_EDITOR
        RegisterModule( new ObjectEditor );
        #endif
        #ifdef MODULE_CAMERA_TOOL
        RegisterModule( new CameraTool );
        #endif
        #ifdef MODULE_COM_KEYBINDS
        RegisterModule( new COMKeyBinds );
        #endif
        #ifdef MODULE_PERSISTENCY
        RegisterModule( new PersistencyModule );
        #endif
        #ifdef MODULE_DEBUG_MONITOR
        RegisterModule( new CustomDebugMonitor );
        #endif
    }

    void ReloadSettings()
    {
        for ( int i = 0; i < m_Modules.Count(); ++i)
        {
            // m_Modules.Get(i).ReloadSettings();
        }
    }

    array< ref Module > GetModules()
    {
        return m_Modules;
    }

    void OnInit()
    {
        for ( int i = 0; i < m_Modules.Count(); ++i)
        {
            m_Modules.Get(i).Init();
        }

        GetUApi().UpdateControls();
    }

    void OnMissionStart()
    {
        for ( int i = 0; i < m_Modules.Count(); ++i)
        {
            m_Modules.Get(i).onMissionStart();
        }
    }

    void OnMissionFinish()
    {
        for ( int i = 0; i < m_Modules.Count(); ++i)
        {
            m_Modules.Get(i).onMissionFinish();
        }
    }

    void OnMissionLoaded()
    {
        for ( int i = 0; i < m_Modules.Count(); ++i)
        {
            m_Modules.Get(i).onMissionLoaded();
        }
    }

    void OnUpdate( float timeslice )
    {
        //if ( GetGame().IsServer() && GetGame().IsMultiplayer() ) return;

		bool inputIsFocused = false;

		Widget focusedWidget = GetFocus();
		if ( focusedWidget && focusedWidget.ClassName().Contains( "EditBoxWidget" ) )
		{
			inputIsFocused = true;
		}

        for ( int i = 0; i < m_Modules.Count(); ++i)
        {
            Module module = m_Modules.Get(i);

            if ( !module.IsPreventingInput() )
            {
                auto bindings = module.GetBindings();

                for ( int nBinding = 0; nBinding < bindings.Count(); ++nBinding )
                {
                    auto k_m_Binding = bindings[ nBinding ];

                    if ( !k_m_Binding.CanBeUsedInMenu() && GetGame().GetUIManager().GetMenu())
                    {
                        continue;
                    }

                    if ( inputIsFocused )
                    {
                        continue;
                    }

                    UAInput input = GetUApi().GetInputByName( k_m_Binding.GetUAInputName() );

                    //COM_Message( k_m_Binding.GetUAInputName() + " -> " + input.LocalPress() );

                    //input.ForceEnable( true );

                    int action = k_m_Binding.GetActionType();

                    if ( action & KeyMouseActionType.PRESS && input.LocalPress() )
                    {
                        GetGame().GameScript.CallFunction( GetModule( k_m_Binding.GetObject() ), k_m_Binding.GetCallBackFunction(), NULL, 0 );
                    }

                    if ( action & KeyMouseActionType.RELEASE && input.LocalRelease() )
                    {
                        GetGame().GameScript.CallFunction( GetModule( k_m_Binding.GetObject() ), k_m_Binding.GetCallBackFunction(), NULL, 0 );
                    }

                    if ( action & KeyMouseActionType.HOLD && input.LocalHold() )
                    {
                        GetGame().GameScript.CallFunction( GetModule( k_m_Binding.GetObject() ), k_m_Binding.GetCallBackFunction(), NULL, 0 );
                    }

                    if ( action & KeyMouseActionType.DOUBLECLICK && input.LocalDoubleClick() )
                    {
                        GetGame().GameScript.CallFunction( GetModule( k_m_Binding.GetObject() ), k_m_Binding.GetCallBackFunction(), NULL, 0 );
                    }

                    if ( action & KeyMouseActionType.VALUE && input.LocalValue() != 0 ) 
                    {
                        GetGame().GameScript.CallFunction( GetModule( k_m_Binding.GetObject() ), k_m_Binding.GetCallBackFunction(), NULL, input.LocalValue() );
                    }
                }
            }

            module.onUpdate( timeslice );
        }
    }

    Module GetModule( typename module_Type )
    {
        for ( int i = 0; i < m_Modules.Count(); ++i )
        {
            Module module = m_Modules.Get(i);

            if ( module.GetModuleType() == module_Type)
            {
                return module;
            }
        }

        return NULL;
    }

    Module GetModuleByName( string module_name )
    {
        for ( int i = 0; i < m_Modules.Count(); ++i )
        {
            Module module = m_Modules.Get( i );

            if (module.GetModuleName() == module_name)
            {
                return module;
            }
        }

        return NULL;
    }
}

ref ModuleManager g_com_ModuleManager;

ModuleManager COM_GetModuleManager()
{
    if( !g_com_ModuleManager )
    {
        g_com_ModuleManager = new ModuleManager();
    }

    return g_com_ModuleManager;
}

ModuleManager NewModuleManager()
{
    if ( g_com_ModuleManager )
    {
        delete g_com_ModuleManager;
    }

    g_com_ModuleManager = new ModuleManager();

    return g_com_ModuleManager;
}
// ===== core/StaticFunctions.c =====
static string COM_FormatFloat( float value, int decimals ) 
{
    if ( !value.ToString().Contains(".") ) return value.ToString();
	
    string result = "";
    array<string> output = new array<string>;

    value.ToString().Split(".", output);

    if ( output.Count() == 0 ) return value.ToString();

    if ( decimals == 0 ) return output.Get(0);

    string right = output.Get(1).Substring(0, decimals);
    result = output.Get(0) + "." + right;
    return result;
}

static string COM_VectorToString( vector vec )
{
    string result = vec.ToString();
    result.Replace( "<", "" );
    result.Replace( ">", "" );
    result.Replace( ",", "" );

    return result;
}

static string COM_VectorToString( vector vec, int decimals ) 
{
    string result = "";
    result = COM_FormatFloat(vec[0], decimals) + "|" + COM_FormatFloat(vec[1], decimals) + "|" + COM_FormatFloat(vec[2], decimals);

    return result;
}

static TStringArray COM_GetChildrenFromBaseClass( string strConfigName, string strBaseClass )
{
    string child_name = "";
    int count = GetGame().ConfigGetChildrenCount ( strConfigName );
    TStringArray class_names = new TStringArray;

    for (int p = 0; p < count; p++)
    {
        GetGame().ConfigGetChildName ( strConfigName, p, child_name );

        if ( GetGame().IsKindOf(child_name, strBaseClass ) && ( child_name != strBaseClass ) )
        {
            class_names.Insert(child_name);
        }
    }

    return class_names;
}

static TVectorArray COM_GetSpawnPoints()
{
    return { "1509.023926 182.689377 7440.780762" };
}

static set< Object > COM_GetObjectsAt( vector from, vector to, Object ignore = NULL, float radius = 0.5, Object with = NULL )
{
    vector contact_pos;
    vector contact_dir;
    int contact_component;

    set< Object > geom = new set< Object >;
    set< Object > view = new set< Object >;

    DayZPhysics.RaycastRV( from, to, contact_pos, contact_dir, contact_component, geom, with, ignore, false, false, ObjIntersectGeom, radius );
    DayZPhysics.RaycastRV( from, to, contact_pos, contact_dir, contact_component, view, with, ignore, false, false, ObjIntersectView, radius );

    if ( geom.Count() > 0 ) 
    {
        return geom;
    }
    if ( view.Count() > 0 ) 
    {
        return view;
    }
    return NULL;
}

static Object COM_GetPointerObject( Object ignore = NULL, float radius = 0.5, Object with = NULL )
{
    vector dir = GetGame().GetPointerDirection();

    vector from = GetGame().GetCurrentCameraPosition();

    vector to = from + ( dir * 10000 );

    auto objs = COM_GetObjectsAt( from, to, ignore, radius, with );

    if( objs.Count() > 0 )
    {
        return objs[ 0 ];
    }

    return NULL;
}

static Object COM_GetCursorObject()
{
    vector rayStart = GetGame().GetCurrentCameraPosition();
    vector rayEnd = rayStart + GetGame().GetCurrentCameraDirection() * 10000;

    auto objs = COM_GetObjectsAt( rayStart, rayEnd );

    if( objs.Count() > 0 )
    {
        return objs[ 0 ];
    }

    return NULL;
}

static vector COM_GetPointerPos()
{
    if ( !COM_GetPB() )
    {
        return "0 0 0";
    }

    vector dir = GetGame().GetPointerDirection();

    vector from = GetGame().GetCurrentCameraPosition();

    vector to = from + ( dir * 10000 );

    vector rayStart = from;
    vector rayEnd = to;
    vector hitPos;
    vector hitNormal;
    int hitComponentIndex;
    DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, hitComponentIndex, NULL, NULL, COM_GetPB());

    return hitPos;
}

static vector COM_GetCursorPos()
{
    if ( !COM_GetPB() )
    {
        return "0 0 0";
    }

    vector rayStart = GetGame().GetCurrentCameraPosition();
    vector rayEnd = rayStart + GetGame().GetCurrentCameraDirection() * 10000;
    vector hitPos;
    vector hitNormal;
    int hitComponentIndex;
    DayZPhysics.RaycastRV(rayStart, rayEnd, hitPos, hitNormal, hitComponentIndex, NULL, NULL, COM_GetPB());

    return hitPos;
}

static void COM_Message( string txt ) 
{
    COM_GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", txt, ""));
}

static Weapon COM_GetWeaponInHands()
{
    Weapon weapon_in_hands;
    if( COM_GetPB() && COM_GetPB().GetItemInHands() ) Class.CastTo(weapon_in_hands,  COM_GetPB().GetItemInHands());

    return weapon_in_hands;
}

static MissionBase COM_GetMission()
{
    return MissionBase.Cast( GetGame().GetMission() );
}

static CommunityOfflineClient COM_GetClientMission()
{
    return CommunityOfflineClient.Cast( GetGame().GetMission() );
}

static CommunityOfflineServer COM_GetServerMission()
{
    return CommunityOfflineServer.Cast( GetGame().GetMission() );
}

static PlayerBase COM_GetPB()
{
    return PlayerBase.Cast( GetGame().GetPlayer() );
}

static bool COM_SHIFT()
{
    return( ( KeyState( KeyCode.KC_LSHIFT ) > 0 ) || ( KeyState( KeyCode.KC_RSHIFT ) > 0 ) );
}

static bool COM_CTRL()
{
    return( ( KeyState( KeyCode.KC_LCONTROL ) > 0 ) || ( KeyState( KeyCode.KC_RCONTROL ) > 0 ) );
}

static bool COM_ALT()
{
    return( ( KeyState( KeyCode.KC_LMENU ) > 0 ) || ( KeyState( KeyCode.KC_RMENU ) > 0 ) );
}

static bool COM_WINKEY()
{
    return( ( KeyState( KeyCode.KC_LWIN ) > 0 ) || ( KeyState( KeyCode.KC_RWIN ) > 0 ) );
}

/*
static Weapon_Base COM_CreateWeapon( PlayerBase oPlayer )
{
    Weapon_Base oWpn = Weapon_Base.Cast(oPlayer.GetInventory().CreateInInventory( "M4A1_Black" ));
    oWpn.GetInventory().CreateAttachment( "M4_Suppressor" );
    oWpn.GetInventory().CreateAttachment( "M4_RISHndgrd_Black" );
    oWpn.GetInventory().CreateAttachment( "M4_MPBttstck_Black" );
    oWpn.GetInventory().CreateAttachment( "ACOGOptic" );

    return oWpn;
}
*/

static Weapon_Base COM_CreateWeapon( PlayerBase oPlayer, string sWeapon )
{
    Weapon_Base oWpn = Weapon_Base.Cast(oPlayer.GetInventory().CreateInInventory( sWeapon ));
    oWpn.GetInventory().CreateAttachment( "PistolSuppressor" );
    EntityAI optic = oWpn.GetInventory().CreateAttachment( "ReflexOptic" );
    optic.GetInventory().CreateAttachment("Battery9V");

    return oWpn;
}

static PlayerBase COM_CreateCustomDefaultCharacter()
{
    PlayerBase oPlayer = PlayerBase.Cast( GetGame().CreatePlayer( NULL, GetGame().CreateRandomPlayer(), COM_GetSpawnPoints().GetRandomElement(), 0, "NONE") );

    oPlayer.GetInventory().CreateInInventory( "MilitaryBeret_UN" );
    oPlayer.GetInventory().CreateInInventory( "M65Jacket_Black" );
    oPlayer.GetInventory().CreateInInventory( "TacticalGloves_Black" );
    oPlayer.GetInventory().CreateInInventory( "HunterPants_Autumn" );
    oPlayer.GetInventory().CreateInInventory( "MilitaryBoots_Black" );
    oPlayer.GetInventory().CreateInInventory( "AliceBag_Camo" );
    oPlayer.GetInventory().CreateInInventory( "Shovel" );

    Weapon_Base oWpn = COM_CreateWeapon( oPlayer, "UMP45" );
    oPlayer.PredictiveTakeEntityToHands( oWpn );

    Magazine oMag = Magazine.Cast( oPlayer.GetInventory().CreateInInventory( "Mag_UMP_25Rnd" ) );
    oPlayer.GetDayZPlayerInventory().PostWeaponEvent( new WeaponEventAttachMagazine( oPlayer, oMag ) );

    oPlayer.GetInventory().CreateInInventory( "Mag_UMP_25Rnd" );

    oPlayer.SetQuickBarEntityShortcut( oWpn, 0, true );
    oPlayer.SetQuickBarEntityShortcut( oMag, 1, true );

    return oPlayer;
}

static string COM_FileAttributeToString( FileAttr attr )
{
    string fileType = "";
    if ( attr & FileAttr.DIRECTORY )
    {
        fileType = fileType + "DIRECTORY";
    }
    if ( attr & FileAttr.HIDDEN )
    {
        fileType = fileType + "HIDDEN";
    }
    if ( attr & FileAttr.READONLY )
    {
        fileType = fileType + "READONLY";
    }
    if ( attr & FileAttr.INVALID )
    {
        fileType = fileType + "INVALID";
    }
    return fileType;
}

static vector COM_SnapToGround(vector pos)
{
    float pos_x = pos[0];
    float pos_z = pos[2];
    float pos_y = GetGame().SurfaceY( pos_x, pos_z );
    vector tmp_pos = Vector( pos_x, pos_y, pos_z );
    tmp_pos[1] = tmp_pos[1] + pos[1];

    return tmp_pos;
}

static bool m_COM_GodMode; // move these to player saves? Edit: Jacob says "yes"
static bool m_COM_OldAiming;
static bool COM_bc_Visible;

static void COM_SnapToGroundNew( Object object ) 
{
    vector pos = object.GetPosition();
    pos[1] = GetGame().SurfaceY(pos[0], pos[2]);
    
    vector clippingInfo[2];
    vector objectBBOX[2];
    
    object.GetCollisionBox( objectBBOX );
    object.ClippingInfo( clippingInfo );

    //float clipY = objectBBOX[1][1] / 2.0//- clippingInfo[0][1];
    //pos[1] = pos[1] + objectBBOX[1][1] - clipY;
    pos[1] = pos[1] + clippingInfo[1][1] / 2.0;//objectBBOX[0][1] - clipY

    object.SetPosition(pos);

    COM_ForceTargetCollisionUpdate( object );
}

static void COM_ForceTargetCollisionUpdate( Object oObj )
{
    if ( !oObj ) return;

    vector roll = oObj.GetOrientation();
    roll [ 2 ] = roll [ 2 ] - 1;
    oObj.SetOrientation( roll );
    roll [ 2 ] = roll [ 2 ] + 1;
    oObj.SetOrientation( roll );
}

static void COM_ToggleCursor()
{
    if ( GetGame().GetInput().HasGameFocus( INPUT_DEVICE_MOUSE ) )
    {
        GetGame().GetInput().ChangeGameFocus( 1 );
        GetGame().GetUIManager().ShowUICursor( true );
    }
    else
    {
        GetGame().GetUIManager().ShowUICursor( false );
        GetGame().GetInput().ResetGameFocus();
    }
}

/*
    Token types:
     0 - error, no token
     1 - defined token (special characters etc. . / * )
     2 - quoted string. Quotes are removed -> TODO
     3 - alphabetic string
     4 - number
     5 - end of line -> TODO
*/
static bool COM_CheckStringType( string str, int type ) 
{
    for(int i = 0; i<str.Length(); i++ ) 
    {
        string character = str.Get(i);
        string token;
        int result = character.ParseStringEx(token);
        if ( result == type ) return true;
    }
    return false;
}

string COM_GetRandomChildFromBaseClass( string strConfigName, string strBaseClass, int minScope = -1 )
{
    string child_name = "";
    int count = GetGame().ConfigGetChildrenCount ( strConfigName );
    array< string > class_names = new array<string>;

    for ( int p = 0; p < count; p++ )
    {
        GetGame().ConfigGetChildName( strConfigName, p, child_name );

        if( ( minScope != -1 ) && ( GetGame().ConfigGetInt( strConfigName + " " + child_name + " scope" ) < minScope ) ) continue;

        if ( GetGame().IsKindOf( child_name, strBaseClass ) && ( child_name != strBaseClass ) )
        {
            class_names.Insert( child_name );
        }
    }

    return class_names.GetRandomElement();
}

// ===== core/CommunityOfflineClient.c =====
class CommunityOfflineClient extends MissionGameplay
{
	protected bool HIVE_ENABLED = true; //Local Hive / Economy / Infected spawn

    protected bool m_loaded;

	void CommunityOfflineClient()
	{
	    m_loaded = false;

		NewModuleManager();
	}

	override void OnInit()
	{
		super.OnInit();

        InitHive();

        SetupWeather();

		SpawnPlayer();

		GetDayZGame().SetMissionPath( "$saves:CommunityOfflineMode\\" ); // CameraToolsMenu
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();

        COM_GetModuleManager().OnInit();
		COM_GetModuleManager().OnMissionStart();
	}

	override void OnMissionFinish()
	{
        COM_GetModuleManager().OnMissionFinish();

		CloseAllMenus();

		DestroyAllMenus();

		if( GetHive() )
		{
			DestroyHive();
		}

		super.OnMissionFinish();
	}

    void COM_OnMissionLoaded()
    {
		COM_GetModuleManager().OnMissionLoaded();
    }

	override void OnUpdate( float timeslice )
	{
	    super.OnUpdate( timeslice );

        COM_GetModuleManager().OnUpdate( timeslice );

        if( !m_loaded && !GetDayZGame().IsLoading() )
        {
            m_loaded = true;
            COM_OnMissionLoaded();
        }
	}

    void SpawnPlayer()
    {
//		#ifndef MODULE_PERSISTENCY
//		GetGame().SelectPlayer( NULL, COM_CreateCustomDefaultCharacter() );
//		#endif

//		#ifdef DISABLE_PERSISTENCY
		GetGame().SelectPlayer( NULL, COM_CreateCustomDefaultCharacter() );
//		#endif
    }

	void InitHive()
	{
		if ( GetGame().IsClient() && GetGame().IsMultiplayer() ) return;

		// RD /s /q "storage_-1" > nul 2>&1
		if ( !HIVE_ENABLED ) return;
	
		Hive oHive = GetHive();
		
		if( !oHive )
		{
			oHive = CreateHive();
		}

		if( oHive )
		{
			oHive.InitOffline();
		}

		oHive.SetShardID("100");
		oHive.SetEnviroment("stable");
	}

    static void SetupWeather()
    {
        Weather weather = g_Game.GetWeather();

        weather.GetOvercast().SetLimits( 0.0 , 2.0 );
        weather.GetRain().SetLimits( 0.0 , 2.0 );
        weather.GetFog().SetLimits( 0.0 , 2.0 );

        weather.GetOvercast().SetForecastChangeLimits( 0.0, 0.0 );
        weather.GetRain().SetForecastChangeLimits( 0.0, 0.0 );
        weather.GetFog().SetForecastChangeLimits( 0.0, 0.0 );

        weather.GetOvercast().SetForecastTimeLimits( 1800 , 1800 );
        weather.GetRain().SetForecastTimeLimits( 600 , 600 );
        weather.GetFog().SetForecastTimeLimits( 600 , 600 );

        weather.GetOvercast().Set( 0.0, 0, 0 );
        weather.GetRain().Set( 0.0, 0, 0 );
        weather.GetFog().Set( 0.0, 0, 0 );

        weather.SetWindMaximumSpeed( 50 );
        weather.SetWindFunctionParams( 0, 0, 1 );
    }
    
    override UIScriptedMenu CreateScriptedMenu(int id)
    {
        if(id == EditorMenu.MENU_ID)
        {
            return new EditorMenu();
        }
        
        return super.CreateScriptedMenu(id);
    }
}

// ===== core/CommunityOfflineServer.c =====
class CommunityOfflineServer : MissionServer
{
    protected bool m_loaded;

	void CommunityOfflineServer()
	{
	    Print( "CommunityOfflineServer::CommunityOfflineServer()" );
	    m_loaded = false;

		COM_GetModuleManager();
	}

	void ~CommunityOfflineServer()
	{
	    Print( "CommunityOfflineServer::~CommunityOfflineServer()" );
	}

	override void OnInit()
	{
		super.OnInit();

        SetupWeather();

        COM_GetModuleManager().OnInit();
	}

	override void OnMissionStart()
	{
		super.OnMissionStart();
	
        COM_GetModuleManager().OnMissionStart();
	}

	override void OnMissionFinish()
	{
        COM_GetModuleManager().OnMissionFinish();

		super.OnMissionFinish();
	}

    void COM_OnMissionLoaded()
    {
		COM_GetModuleManager().OnMissionLoaded();
    }


	override void OnUpdate( float timeslice )
	{
	    super.OnUpdate( timeslice );

        COM_GetModuleManager().OnUpdate( timeslice );

        if( !m_loaded && !GetDayZGame().IsLoading() )
        {
            m_loaded = true;
            COM_OnMissionLoaded();
        }
	}

	override void OnMouseButtonRelease( int button )
	{
		super.OnMouseButtonRelease( button );

		//COM_GetModuleManager().OnMouseButtonRelease( button );
	}

	override void OnMouseButtonPress( int button )
	{
		super.OnMouseButtonPress( button );

		//COM_GetModuleManager().OnMouseButtonPress( button );
	}

	override void OnKeyPress( int key )
	{
		super.OnKeyPress(key);

		//COM_GetModuleManager().OnKeyPress( key );
    }

    override void OnKeyRelease( int key )
	{
		super.OnKeyRelease( key );

		//COM_GetModuleManager().OnKeyRelease( key );
	}

    static void SetupWeather()
    {
        //Offical DayZ SA weather code
        Weather weather = g_Game.GetWeather();

        weather.GetOvercast().SetLimits( 0.0 , 2.0 );
        weather.GetRain().SetLimits( 0.0 , 2.0 );
        weather.GetFog().SetLimits( 0.0 , 2.0 );

        weather.GetOvercast().SetForecastChangeLimits( 0.0, 0.0 );
        weather.GetRain().SetForecastChangeLimits( 0.0, 0.0 );
        weather.GetFog().SetForecastChangeLimits( 0.0, 0.0 );

        weather.GetOvercast().SetForecastTimeLimits( 1800 , 1800 );
        weather.GetRain().SetForecastTimeLimits( 600 , 600 );
        weather.GetFog().SetForecastTimeLimits( 600 , 600 );

        weather.GetOvercast().Set( 0.0, 0, 0 );
        weather.GetRain().Set( 0.0, 0, 0 );
        weather.GetFog().Set( 0.0, 0, 0 );

        weather.SetWindMaximumSpeed( 50 );
        weather.SetWindFunctionParams( 0, 0, 1 );
    }
    
    override UIScriptedMenu CreateScriptedMenu(int id)
    {
        if(id == EditorMenu.MENU_ID)
        {
            return new EditorMenu();
        }
        
        return super.CreateScriptedMenu(id);
    }
}



  
Mission CreateCustomMission(string path)
{	
    if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
        return new CommunityOfflineServer();

    return new CommunityOfflineClient();
}
