// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  void zCXInputDevice::InitCombinations() {
#pragma region arrow pad normal
    // Open status screen
    KEYRECORD_BEGIN
      COMBINATION ArrowLeft
      EMULATION   GetBinded( GAME_SCREEN_STATUS )
      CONDITION   IsNotInInterface
    KEYRECORD_END

    // Open log
    KEYRECORD_BEGIN
      COMBINATION ArrowRight
      EMULATION   GetBinded( GAME_SCREEN_LOG )
      CONDITION   IsNotInInterface
    KEYRECORD_END

    // Open magic
    KEYRECORD_BEGIN
      COMBINATION ArrowUp
      EMULATION   KEY_3
      CONDITION   IsNotInInterface
    KEYRECORD_END

    // Open map
    KEYRECORD_BEGIN
      COMBINATION ArrowDown
      EMULATION   GetBinded( GAME_SCREEN_MAP )
      CONDITION   IsNotInInterface
    KEYRECORD_END
#pragma endregion


#pragma region arrow pad in interface
    // Arrow left
    KEYRECORD_BEGIN
      COMBINATION ArrowLeft
      EMULATION   GetBinded( GAME_LEFT )
      CONDITION   IsInInterface
    KEYRECORD_END

    // Arrow right
    KEYRECORD_BEGIN
      COMBINATION ArrowRight
      EMULATION   GetBinded( GAME_RIGHT )
      CONDITION   IsInInterface
    KEYRECORD_END

    // Arrow up
    KEYRECORD_BEGIN
      COMBINATION ArrowUp
      EMULATION   GetBinded( GAME_UP )
      CONDITION   IsInInterface
    KEYRECORD_END

    // Arrow down
    KEYRECORD_BEGIN
      COMBINATION ArrowDown
      EMULATION   GetBinded( GAME_DOWN )
      CONDITION   IsInInterface
    KEYRECORD_END
#pragma endregion


#pragma region arrow pad in inventory
    // Arrow left
    KEYRECORD_BEGIN
      COMBINATION ArrowLeft
      EMULATION   GetBinded( GAME_STRAFELEFT )
      CONDITION   IsInInventory
    KEYRECORD_END

    // Arrow right
    KEYRECORD_BEGIN
      COMBINATION ArrowRight
      EMULATION   GetBinded( GAME_STRAFERIGHT )
      CONDITION   IsInInventory
    KEYRECORD_END

    // Arrow up
    KEYRECORD_BEGIN
      COMBINATION ArrowUp
      EMULATION   GetBinded( GAME_UP )
      CONDITION   IsInInventory
    KEYRECORD_END

    // Arrow down
    KEYRECORD_BEGIN
      COMBINATION ArrowDown
      EMULATION   GetBinded( GAME_DOWN )
      CONDITION   IsInInventory
    KEYRECORD_END
#pragma endregion


#pragma region arrow pad in fight
    // Arrow left
    // Next left enemy
    KEYRECORD_BEGIN
      COMBINATION ArrowLeft
      EMULATION   KEY_F14
      CONDITION   IsFightMode
    KEYRECORD_END

    // Arrow right
    // Next right enemy
    KEYRECORD_BEGIN
      COMBINATION ArrowRight
      EMULATION   KEY_F15
      CONDITION   IsFightMode
    KEYRECORD_END
#pragma endregion


#pragma region button A (X dualshok) normal
    // Button A
    KEYRECORD_BEGIN
      COMBINATION ButtonCross
      EMULATION   MOUSE_BUTTONLEFT
      CONDITION   IsNotInTransform
    KEYRECORD_END
#pragma endregion


#pragma region button A in transform
    // Button A (cross on dualshok) 
    KEYRECORD_BEGIN
      COMBINATION ButtonCross
      EMULATION   KEY_RETURN
      CONDITION   IsInTransform
    KEYRECORD_END
#pragma endregion


#pragma region button pad
    // Button B (round on dualshok) - back
    KEYRECORD_BEGIN
      COMBINATION ButtonCross
      EMULATION   MOUSE_BUTTONLEFT
      CONDITION   Null
    KEYRECORD_END

    // Button X (square on dualshok) - jump
    KEYRECORD_BEGIN
      COMBINATION ButtonSquare
      EMULATION   GetBinded( GAME_SMOVE )
      CONDITION   Null
    KEYRECORD_END

    // Button Y (triangle on dualshok) - draw/remove weapon
    KEYRECORD_BEGIN
      COMBINATION ButtonTriangle
      EMULATION   GetBinded( GAME_WEAPON )
      CONDITION   Null
    KEYRECORD_END

    // Button Y (triangle on dualshok) - draw/remove weapon
    KEYRECORD_BEGIN
      COMBINATION ButtonCircle
      EMULATION   MOUSE_BUTTONRIGHT
      CONDITION   IsNotPlayVideo
    KEYRECORD_END

    // Button Menu (start on dualshok) - menu
    KEYRECORD_BEGIN
      COMBINATION ButtonStart
      EMULATION   GetBinded( GAME_END )
      CONDITION   Null
    KEYRECORD_END

    // Button View (select on dualshok) - inventory
    KEYRECORD_BEGIN
      COMBINATION ButtonSelect
      EMULATION   GetBinded( GAME_INVENTORY )
      CONDITION   Null
    KEYRECORD_END
#pragma endregion


#pragma region on video
      // Button Y (triangle on dualshok) - draw/remove weapon
      KEYRECORD_BEGIN
      COMBINATION ButtonCircle
      EMULATION   KEY_ESCAPE
      CONDITION   IsPlayVideo
      KEYRECORD_END
#pragma endregion


#pragma region left stick
    // Walk
    KEYRECORD_BEGIN
      COMBINATION GameWalk
      EMULATION   GetBinded( GAME_UP ), GetBinded( GAME_SLOW )
      CONDITION   Null
    KEYRECORD_END
      
    // Run forward
    KEYRECORD_BEGIN
      COMBINATION GameForward
      EMULATION   GetBinded( GAME_UP )
      CONDITION   Null
    KEYRECORD_END
      
    // Run backward
    KEYRECORD_BEGIN
      COMBINATION GameBackward
      EMULATION   GetBinded( GAME_DOWN )
      CONDITION   Null
    KEYRECORD_END
      
    // Run left strafe
    KEYRECORD_BEGIN
      COMBINATION GameLeftStrafe
      EMULATION   GetBinded( GAME_STRAFELEFT )
      CONDITION   Null
    KEYRECORD_END
      
    // Run right strafe
    KEYRECORD_BEGIN
      COMBINATION GameRightStrafe
      EMULATION   GetBinded( GAME_STRAFERIGHT )
      CONDITION   Null
    KEYRECORD_END
      
    // Snake
    KEYRECORD_BEGIN
      COMBINATION ButtonLeftStick
      EMULATION   GetBinded( GAME_SNEAK )
      CONDITION   Null
    KEYRECORD_END
#pragma endregion


#pragma region triggers normal
    // Draw melee weapon
    KEYRECORD_BEGIN
      COMBINATION GamePunch
      EMULATION   KEY_1
      CONDITION   IsNotFightMode
    KEYRECORD_END
    
    // Draw range weapon
    KEYRECORD_BEGIN
      COMBINATION GameParade
      EMULATION   KEY_2
      CONDITION   IsNotFightMode
    KEYRECORD_END
#pragma endregion


#pragma region triggers melee
    // Draw melee weapon
    KEYRECORD_BEGIN
      COMBINATION GamePunch
      EMULATION   GetBinded( GAME_UP ), GetBinded( GAME_ACTION )
      CONDITION   IsFightModeMelee
    KEYRECORD_END

    // Draw range weapon
    KEYRECORD_BEGIN
      COMBINATION GameParade
      EMULATION   GetBinded( GAME_DOWN ), GetBinded( GAME_ACTION )
      CONDITION   IsFightModeMelee
    KEYRECORD_END
#pragma endregion


#pragma region triggers range
    // Draw melee weapon
    KEYRECORD_BEGIN
      COMBINATION GamePunch
      EMULATION   GetBinded( GAME_UP )
      CONDITION   IsFightModeRange
    KEYRECORD_END

    // Draw range weapon
    KEYRECORD_BEGIN
      COMBINATION GameParade
      EMULATION   GetBinded( GAME_ACTION )
      CONDITION   IsFightModeRange
    KEYRECORD_END
#pragma endregion


#pragma region other fight mode
    // Forward punch
    KEYRECORD_BEGIN
      COMBINATION GameForward, GamePunch
      EMULATION   GetBinded( GAME_ACTION )
      CONDITION   IsFightModeMelee
    KEYRECORD_END
      
    // Left punch
    KEYRECORD_BEGIN
      COMBINATION ButtonL1
      EMULATION   GetBinded( GAME_LEFT ), GetBinded( GAME_ACTION )
      CONDITION   IsFightModeMelee
    KEYRECORD_END
      
    // Right punch
    KEYRECORD_BEGIN
      COMBINATION ButtonR1
      EMULATION   GetBinded( GAME_RIGHT ), GetBinded( GAME_ACTION )
      CONDITION   IsFightModeMelee
    KEYRECORD_END
      
    // First person
    KEYRECORD_BEGIN
      COMBINATION ButtonRightStick, ArrowDown
      EMULATION   GetBinded( GAME_LOOK_FP )
      CONDITION   Null
    KEYRECORD_END
      
    // Lock target
    KEYRECORD_BEGIN
      COMBINATION ButtonRightStick
      EMULATION   GetBinded( GAME_LOCK_TARGET )
      CONDITION   HasLockedTarget
    KEYRECORD_END
#pragma endregion


#pragma region quick potion key
    // Mana potion
    KEYRECORD_BEGIN
      COMBINATION ButtonL1
      EMULATION   GetBinded( GAME_LAME_POTION )
      CONDITION   IsCanDrink
    KEYRECORD_END
    
    // Heal potion
    KEYRECORD_BEGIN
      COMBINATION ButtonR1
      EMULATION   GetBinded( GAME_LAME_HEAL )
      CONDITION   IsCanDrink
    KEYRECORD_END
#pragma endregion


#pragma region inventory shell
    // Prev category
    KEYRECORD_BEGIN
      COMBINATION ButtonL1
      EMULATION   KEY_LSHIFT, GetBinded( GAME_LEFT )
      CONDITION   IsInInventory
    KEYRECORD_END
    
    // Next category
    KEYRECORD_BEGIN
      COMBINATION ButtonR1
      EMULATION   KEY_LSHIFT, GetBinded( GAME_RIGHT )
      CONDITION   IsInInventory
    KEYRECORD_END
#pragma endregion
  }
}