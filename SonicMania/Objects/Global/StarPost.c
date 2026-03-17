// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: StarPost Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectStarPost *StarPost;

void StarPost_Update(void)
{
    RSDK_THIS(StarPost);
    StateMachine_Run(self->state);
}

void StarPost_LateUpdate(void) {}

void StarPost_StaticUpdate(void) {}

void StarPost_Draw(void)
{
    RSDK_THIS(StarPost);

    RSDK.DrawSprite(&self->poleAnimator, &self->position, false);

    self->ballPos.x = self->position.x - 0x280 * RSDK.Cos1024(self->angle);
    self->ballPos.y = self->position.y - 0x280 * RSDK.Sin1024(self->angle) - TO_FIXED(14);
    RSDK.DrawSprite(&self->ballAnimator, &self->ballPos, false);

    Vector2 drawPos;
    if (self->bonusStageID > 0) {
        int32 angleX    = self->starAngleX;
        int32 amplitude = 3 * RSDK.Sin512(self->starAngleY);
        for (int32 i = 0; i < 4; ++i) {
            drawPos.x = self->position.x + ((RSDK.Sin512(angleX) << 12) * self->starRadius >> 7);
            drawPos.y = (((amplitude * RSDK.Sin512(angleX)) + (RSDK.Cos512(angleX) << 10)) * self->starRadius >> 7) + self->position.y - TO_FIXED(50);
            RSDK.DrawSprite(&self->starAnimator, &drawPos, false);
            angleX += 128;
        }
    }
}

void StarPost_Create(void *data)
{
    RSDK_THIS(StarPost);

    self->visible   = true;
    self->drawGroup = 2;
    self->active    = ACTIVE_BOUNDS;
    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;

    // FIX: Set default angle so it points straight up! 
    // Without this, it defaults to 0 (sideways/mid-animation)
    self->angle = 0x100;

    RSDK.SetSpriteAnimation(StarPost->aniFrames, 0, &self->poleAnimator, true, 0);
    RSDK.SetSpriteAnimation(StarPost->aniFrames, 1, &self->ballAnimator, true, 0);
    RSDK.SetSpriteAnimation(StarPost->aniFrames, 3, &self->starAnimator, true, 0);

    // WASM SAFE: Only check slot 0!
    if (self->id > 0 && globals->checkpointID[0] == self->id) {
        self->interactedPlayers |= 1; 
        self->state = StarPost_State_Idle;
        RSDK.SetSpriteAnimation(StarPost->aniFrames, 2, &self->ballAnimator, true, 0);
        self->ballAnimator.speed = 64;

        // FIX: Teleport the player ONLY ONCE when the post loads (Respawning)
        EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (player) {
            player->position.x = self->position.x;
            player->position.y = self->position.y;
            player->velocity.x = 0;
            player->velocity.y = 0;
            player->groundVel  = 0;

            if (player->camera) {
                player->camera->position.x = self->position.x;
                player->camera->position.y = self->position.y;
            }

            // Sync Stage Timer
            SceneInfo->minutes      = globals->checkpointMinutes;
            SceneInfo->seconds      = globals->checkpointSeconds;
            SceneInfo->milliseconds = globals->checkpointMilliseconds;
        }
    } else {
        self->state = StarPost_State_Idle;
    }

    if (data) {
        self->state = (void (*)(void))data;
    }
}

void StarPost_StageLoad(void)
{
    StarPost->aniFrames = RSDK.LoadSpriteAnimation("Global/StarPost.bin", SCOPE_STAGE);

    // --- HITBOX FIXED HERE ---
    // Top and bottom extended to 52 to cover the entire pole down to the floor
    StarPost->hitbox.left   = -8;
    StarPost->hitbox.top    = -52; 
    StarPost->hitbox.right  = 8;
    StarPost->hitbox.bottom = 52;  // This was 0! It was letting you roll right under it!
    // -------------------------

    StarPost->sfxStarPost = RSDK.GetSfx("Global/StarPost.wav");
    StarPost->sfxWarp     = RSDK.GetSfx("Global/Warp.wav");
    StarPost->interactablePlayers = PLAYER_COUNT;
}

void StarPost_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(StarPost->aniFrames, 0, &DebugMode->animator, true, 1);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void StarPost_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(StarPost, NULL, self->position.x, self->position.y);
}

void StarPost_ResetStarPosts(void)
{
    // WASM SAFE: Array is size 4, so loop max is 4
    for (int32 p = 0; p < 4; ++p) {
        globals->checkpointID[p] = 0;
    }
    globals->checkpointMilliseconds = 0;
    globals->checkpointSeconds      = 0;
    globals->checkpointMinutes      = 0;
}

void StarPost_CheckBonusStageEntry(void)
{
    RSDK_THIS(StarPost);

    self->starRadius = (MathHelpers_Sin256(self->starAngle) >> 4) + 24;
    self->starAngle += 4;

    if (self->starTimer < 60)
        self->starTimer++;

    if (self->starTimer >= 60) {
        if (!globals->recallEntities) {
            // Check if Player 1 touches the stars
            if (Player_CheckCollisionTouch(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), self, &self->hitboxStars)) {
                
                SaveGame_SaveGameState(); 
                
                // --- THE TRUE MEMORY FIX FOR STARPOST ---
                SaveRAM *saveRAM = NULL;
                if (globals->saveSlotID == NO_SAVE_SLOT) {
                    saveRAM = (SaveRAM *)globals->noSaveSlot;
                }
                else {
#if MANIA_USE_PLUS
                    saveRAM = (SaveRAM *)SaveGame_GetDataPtr(globals->saveSlotID, globals->gameMode == MODE_ENCORE);
#else
                    saveRAM = (SaveRAM *)SaveGame_GetDataPtr(globals->saveSlotID);
#endif
                }
                
                if (saveRAM) {
                    saveRAM->storedStageID = SceneInfo->listPos;
                }
                // ----------------------------------------

                RSDK.PlaySfx(StarPost->sfxWarp, false, 0xFE);
                RSDK.SetEngineState(ENGINESTATE_FROZEN);
                self->state = StarPost_State_BonusWarp;
                self->timer = 0;
            }
        }
    }
}

void StarPost_CheckCollisions(void)
{
    RSDK_THIS(StarPost);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);
        
        // --- SAFE ID SHIELD FOR EXTRA SLOTS ---
        int32 safeID = playerID;
        if (safeID >= 4) {
            safeID = 0; 
        }

        // --- ANTI-TUNNELING HITBOX ---
        // Stretch the hitbox horizontally based on player speed 
        // so they don't skip over it entirely in a single frame!
        Hitbox touchBox = StarPost->hitbox;
        int32 speedX = (player->onGround ? player->groundVel : player->velocity.x) >> 16;
        
        if (speedX < 0) {
            touchBox.left += speedX;  // Expands the left side when moving left
        } else if (speedX > 0) {
            touchBox.right += speedX; // Expands the right side when moving right
        }

        if (!((1 << playerID) & self->interactedPlayers) && !player->sidekick) {
            
            // MAKE SURE TO USE '&touchBox' HERE INSTEAD OF '&StarPost->hitbox'
            if (Player_CheckCollisionTouch(player, self, &touchBox)) {
                self->state = StarPost_State_Spinning;
                self->active = ACTIVE_NORMAL;
                
                if (!TMZ2Setup) {
                    foreach_all(StarPost, starPost)
                    {
                        if (starPost->id < self->id && !starPost->interactedPlayers) {
                            starPost->interactedPlayers = 1 << playerID;
                            RSDK.SetSpriteAnimation(StarPost->aniFrames, 2, &starPost->ballAnimator, true, 0);
                        }
                    }
                }

                // --- SAVING WITH SAFE ID ---
                globals->checkpointID[safeID]    = self->id;
                globals->checkpointPos[safeID].x = self->position.x;
                globals->checkpointPos[safeID].y = self->position.y;
                globals->checkpointDir[safeID]   = self->direction;
                
                if (globals->gameMode < MODE_TIMEATTACK) {
                    globals->checkpointMilliseconds = SceneInfo->milliseconds;
                    globals->checkpointSeconds      = SceneInfo->seconds;
                    globals->checkpointMinutes      = SceneInfo->minutes;
                }
                // ---------------------------

                int32 playerVelocity = player->onGround ? player->groundVel : player->velocity.x;
                int32 ballSpeed      = -12 * (playerVelocity >> 17);

                if (ballSpeed >= 0) ballSpeed += 32;
                else ballSpeed -= 32;

                if (!self->ballSpeed) {
                    self->ballSpeed = ballSpeed;
                }
                else if (self->ballSpeed <= 0) {
                    if (ballSpeed < self->ballSpeed) self->ballSpeed = ballSpeed;
                    else if (ballSpeed > 0) {
                        ballSpeed += self->ballSpeed;
                        self->ballSpeed = ballSpeed;
                    }
                }
                else {
                    if (ballSpeed > self->ballSpeed) self->ballSpeed = ballSpeed;
                    else if (ballSpeed < 0) {
                        ballSpeed += self->ballSpeed;
                        self->ballSpeed = ballSpeed;
                    }
                }

                self->timer = 0;
                if (globals->gameMode < MODE_TIMEATTACK) {
                    int32 quota = 25;
#if MANIA_USE_PLUS
                    if (globals->gameMode == MODE_ENCORE) quota = 50;
#endif
                    if (player->rings >= quota) {
                        self->starTimer  = 0;
                        self->starAngleY = 0;
                        self->starAngleX = 0;
                        self->starRadius = 0;
                        self->bonusStageID = (player->rings - 20) % 3 + 1;
                    }
                }

                if (!self->interactedPlayers) {
                    RSDK.SetSpriteAnimation(StarPost->aniFrames, 2, &self->ballAnimator, true, 0);
                    self->ballAnimator.speed = 0;
                }

                self->interactedPlayers |= 1 << playerID;
                self->active = ACTIVE_NORMAL;
                RSDK.PlaySfx(StarPost->sfxStarPost, false, 255);
            }
        }
    }
}

void StarPost_State_Idle(void)
{
    RSDK_THIS(StarPost);

    if (self->interactedPlayers < PLAYER_COUNT)
        StarPost_CheckCollisions();

    if (self->bonusStageID > 0)
        StarPost_CheckBonusStageEntry();

    RSDK.ProcessAnimation(&self->ballAnimator);
}

void StarPost_State_Spinning(void)
{
    RSDK_THIS(StarPost);

    if (self->interactedPlayers < PLAYER_COUNT)
        StarPost_CheckCollisions();

    self->angle += self->ballSpeed;
    if (!StarPost->hasAchievement && self->timer == 10) {
        API_UnlockAchievement(&achievementList[ACH_STARPOST]);
        StarPost->hasAchievement = true;
    }

    bool32 isIdle = false;
    if (self->ballSpeed <= 0) {
        if (self->angle <= -0x300) {
            ++self->timer;
            self->angle += 0x400;

            self->ballSpeed += 8;
            if (self->ballSpeed > -32)
                self->ballSpeed = -32;

            isIdle = self->ballSpeed == -32;
        }
    }
    else {
        if (self->angle >= 0x500) {
            ++self->timer;
            self->angle -= 0x400;

            self->ballSpeed -= 8;
            if (self->ballSpeed < 32)
                self->ballSpeed = 32;

            isIdle = self->ballSpeed == 32;
        }
    }

    if (isIdle) {
        self->state              = StarPost_State_Idle;
        self->ballAnimator.speed = 64;
        self->ballSpeed          = 0;
        self->angle              = 0x100;

        if (!self->bonusStageID)
            self->active = ACTIVE_BOUNDS;
    }

    if (self->bonusStageID > 0)
        StarPost_CheckBonusStageEntry();

    RSDK.ProcessAnimation(&self->ballAnimator);
}

#if RETRO_INCLUDE_EDITOR
void StarPost_EditorDraw(void) { StarPost_DebugDraw(); }

void StarPost_EditorLoad(void)
{
    StarPost->aniFrames = RSDK.LoadSpriteAnimation("Global/StarPost.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(StarPost, direction);
    RSDK_ENUM_VAR("Respawn Facing Right", FLIP_NONE);
    RSDK_ENUM_VAR("Respawn Facing Left", FLIP_X);
}
#endif

void StarPost_Serialize(void)
{
    RSDK_EDITABLE_VAR(StarPost, VAR_ENUM, id);
    RSDK_EDITABLE_VAR(StarPost, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(StarPost, VAR_BOOL, vsRemove);
}
