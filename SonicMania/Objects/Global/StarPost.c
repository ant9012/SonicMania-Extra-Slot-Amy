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

    if (globals->gameMode == MODE_TIMEATTACK || (globals->gameMode == MODE_COMPETITION && self->vsRemove)) {
        destroyEntity(self);
        return;
    }

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = TO_FIXED(64);
        self->updateRange.y = TO_FIXED(64);
        self->state         = StarPost_State_Idle;
        self->angle         = 0x100; // Reset to default upright angle (256)
    }

    RSDK.SetSpriteAnimation(StarPost->aniFrames, 0, &self->poleAnimator, true, 0);
    
    // Check if this specific starpost is the one saved in globals
    bool32 isSavedPost = false;
for (int32 p = 0; p < PLAYER_COUNT; ++p) {
    // Check against self->id instead of entitySlot
    if (globals->checkpointID[p] == self->id && self->id > 0) {
        isSavedPost = true;
        break;
    }
    }

    if (isSavedPost) {
        self->interactedPlayers = 0xFF; // Mark as hit
        RSDK.SetSpriteAnimation(StarPost->aniFrames, 2, &self->ballAnimator, true, 0);
        self->ballAnimator.speed = 64;
    }
    else {
        RSDK.SetSpriteAnimation(StarPost->aniFrames, 1, &self->ballAnimator, true, 0);
    }
    
    RSDK.SetSpriteAnimation(StarPost->aniFrames, 3, &self->starAnimator, true, 0);
    self->ballPos.x = self->position.x;
    self->ballPos.y = self->position.y - TO_FIXED(24);
}

void StarPost_StageLoad(void)
{
    StarPost->aniFrames = RSDK.LoadSpriteAnimation("Global/StarPost.bin", SCOPE_STAGE);

    StarPost->hitbox.left   = -8;
    StarPost->hitbox.top    = -64;
    StarPost->hitbox.right  = 8;
    StarPost->hitbox.bottom = 0;

    StarPost->sfxStarPost = RSDK.GetSfx("Global/StarPost.wav");
    StarPost->sfxWarp     = RSDK.GetSfx("Global/Warp.wav");

    // Only attempt to spawn at checkpoint if we aren't in a special/cutscene state
    if (SceneInfo->state == ENGINESTATE_REGULAR) {
        for (int32 p = 0; p < PLAYER_COUNT; ++p) {
          // Check globals instead of StarPost statics
          if (globals->checkpointID[p] > 0) {
                printf("WASM_DEBUG: Found Checkpoint ID %d for Player %d\n", globals->checkpointID[p], p);
            if (globals->checkpointID[p]) {
                EntityPlayer *player = RSDK_GET_ENTITY(p, Player);
                
                // Move player to the saved global position
                player->position.x = globals->checkpointPos[p].x;
                player->position.y = globals->checkpointPos[p].y;
                player->direction  = globals->checkpointDir[p];
                
                // Sync the stage timer to the checkpoint time stored in globals
                SceneInfo->minutes      = globals->checkpointMinutes;
                SceneInfo->seconds      = globals->checkpointSeconds;
                SceneInfo->milliseconds = globals->checkpointMilliseconds;
            }
        }
    }
    StarPost->interactablePlayers = PLAYER_COUNT;
    StarPost->hitbox.left   = -8;
    StarPost->hitbox.top    = -64;
    StarPost->hitbox.right  = 8;
    StarPost->hitbox.bottom = 0;
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
    for (int32 i = 0; i < PLAYER_COUNT; ++i) {
        globals->checkpointID[i] = 0;
    }
    globals->checkpointMilliseconds = 0;
    globals->checkpointSeconds      = 0;
    globals->checkpointMinutes      = 0;
}
void StarPost_CheckBonusStageEntry(void)
{
    RSDK_THIS(StarPost);

    self->starAngleY += 4;
    self->starAngleY &= 0x1FF;
    self->starAngleX += 18;
    self->starAngleX &= 0x1FF;

    if (self->starTimer > 472)
        --self->starRadius;
    else if (self->starTimer < 0x80)
        ++self->starRadius;

    if (++self->starTimer == 600) {
        self->starTimer    = 0;
        self->bonusStageID = 0;
        self->active       = ACTIVE_BOUNDS;
    }

    self->starAnimator.frameID = (self->starAngleY >> 3) & 3;

    self->hitboxStars.left   = -(self->starRadius >> 2);
    self->hitboxStars.top    = -48;
    self->hitboxStars.right  = self->starRadius >> 2;
    self->hitboxStars.bottom = -40;

    if (self->starTimer >= 60) {
        if (!globals->recallEntities) {
            if (Player_CheckCollisionTouch(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), self, &self->hitboxStars)) {
                SaveGame_SaveGameState();
                RSDK.PlaySfx(StarPost->sfxWarp, false, 0xFE);
                RSDK.SetEngineState(ENGINESTATE_FROZEN);

#if MANIA_USE_PLUS
                ProgressRAM *progress = GameProgress_GetProgressRAM();
                if ((API.CheckDLC(DLC_PLUS) && progress && progress->allGoldMedals) || globals->gameMode == MODE_ENCORE) {
                    SaveGame_GetSaveRAM()->storedStageID = SceneInfo->listPos;
                    RSDK.SetScene("Pinball", "");
                    Zone_StartFadeOut(10, 0xF0F0F0);
                    Music_Stop();
                }
                else {
#endif
                    SaveGame_GetSaveRAM()->storedStageID = SceneInfo->listPos;
                    RSDK.SetScene("Blue Spheres", "");
                    SceneInfo->listPos += globals->blueSpheresID;
                    Zone_StartFadeOut(10, 0xF0F0F0);
                    Music_Stop();
#if MANIA_USE_PLUS
                }
#endif
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
        if (!((1 << playerID) & self->interactedPlayers) && !player->sidekick) {
            if (Player_CheckCollisionTouch(player, self, &StarPost->hitbox)) {
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

                // --- CHANGED TO GLOBALS HERE ---
                globals->checkpointID[playerID]    = self->id;
                globals->checkpointPos[playerID].x = self->position.x;
                globals->checkpointPos[playerID].y = self->position.y;
                globals->checkpointDir[playerID]   = self->direction;
                
                if (globals->gameMode < MODE_TIMEATTACK) {
                    globals->checkpointMilliseconds = SceneInfo->milliseconds;
                    globals->checkpointSeconds      = SceneInfo->seconds;
                    globals->checkpointMinutes      = SceneInfo->minutes;
                }
                // -------------------------------

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

    if (self->interactedPlayers < StarPost->interactablePlayers)
        StarPost_CheckCollisions();

    if (self->bonusStageID > 0)
        StarPost_CheckBonusStageEntry();

    RSDK.ProcessAnimation(&self->ballAnimator);
}
void StarPost_State_Spinning(void)
{
    RSDK_THIS(StarPost);

    if (self->interactedPlayers < StarPost->interactablePlayers)
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
