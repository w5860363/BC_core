-- BlizzLikeCore TeleNPC
--
-- Remove TeleNPC
--

DELETE FROM creature WHERE guid BETWEEN 1550400 AND 1550408;
DELETE FROM creature_addon WHERE guid BETWEEN 1550400 AND 1550408;
DELETE FROM game_event_creature WHERE guid BETWEEN 1550400 AND 1550408;
DELETE FROM game_event_model_equip WHERE guid BETWEEN 1550400 AND 1550408;

--
-- Spawn Alliance
--

INSERT INTO creature VALUES (1550400,91010,0,1,0,0,-8909.38,-131.39,80.7146,2.57218,25,0,0,5000,0,0,0);
INSERT INTO creature VALUES (1550401,91010,1,1,0,0,10326.8,821.629,1326.45,2.17712,25,0,0,5000,0,0,0);
INSERT INTO creature VALUES (1550402,91010,530,1,0,0,-3994.13,-13881.1,91.8079,5.80959,25,0,0,5000,0,0,0);
INSERT INTO creature VALUES (1550403,91010,0,1,0,0,-6237.42,316.829,382.839,1.03751,25,0,0,5000,0,0,0);

--
-- Spawn Horde
--

INSERT INTO creature VALUES (1550404,91010,1,1,0,0,-607.238,-4248.01,38.9564,3.40313,25,0,0,5000,0,0,0);
INSERT INTO creature VALUES (1550405,91010,530,1,0,0,10356.4,-6376.43,38.3299,1.82919,25,0,0,5000,0,0,0);
INSERT INTO creature VALUES (1550406,91010,1,1,0,0,-2907.83,-258.853,52.9424,3.61755,25,0,0,5000,0,0,0);
INSERT INTO creature VALUES (1550407,91010,0,1,0,0,1681.8,1660.8,135.002,3.02221,25,0,0,5000,0,0,0);

--
-- Spawn GM Island
--

INSERT INTO creature VALUES (1550408,91010,1,1,0,0,16201.9,16256.3,21.2198,0.255988,25,0,0,5000,0,0,0);

--
-- Makes the creatures invisibles
--

-- INSERT INTO `creature_addon`(`guid`,`path_id`,`mount`,`bytes0`,`bytes1`,`bytes2`,`emote`,`moveflags`,`isActive`,`auras`) VALUES (1550400,0,0,0,0,0,0,0,1,'16380 0');
-- INSERT INTO `creature_addon`(`guid`,`path_id`,`mount`,`bytes0`,`bytes1`,`bytes2`,`emote`,`moveflags`,`isActive`,`auras`) VALUES (1550401,0,0,0,0,0,0,0,1,'16380 0');
-- INSERT INTO `creature_addon`(`guid`,`path_id`,`mount`,`bytes0`,`bytes1`,`bytes2`,`emote`,`moveflags`,`isActive`,`auras`) VALUES (1550402,0,0,0,0,0,0,0,1,'16380 0');
-- INSERT INTO `creature_addon`(`guid`,`path_id`,`mount`,`bytes0`,`bytes1`,`bytes2`,`emote`,`moveflags`,`isActive`,`auras`) VALUES (1550403,0,0,0,0,0,0,0,1,'16380 0');
-- INSERT INTO `creature_addon`(`guid`,`path_id`,`mount`,`bytes0`,`bytes1`,`bytes2`,`emote`,`moveflags`,`isActive`,`auras`) VALUES (1550404,0,0,0,0,0,0,0,1,'16380 0');
-- INSERT INTO `creature_addon`(`guid`,`path_id`,`mount`,`bytes0`,`bytes1`,`bytes2`,`emote`,`moveflags`,`isActive`,`auras`) VALUES (1550405,0,0,0,0,0,0,0,1,'16380 0');
-- INSERT INTO `creature_addon`(`guid`,`path_id`,`mount`,`bytes0`,`bytes1`,`bytes2`,`emote`,`moveflags`,`isActive`,`auras`) VALUES (1550406,0,0,0,0,0,0,0,1,'16380 0');
-- INSERT INTO `creature_addon`(`guid`,`path_id`,`mount`,`bytes0`,`bytes1`,`bytes2`,`emote`,`moveflags`,`isActive`,`auras`) VALUES (1550407,0,0,0,0,0,0,0,1,'16380 0');
-- INSERT INTO `creature_addon`(`guid`,`path_id`,`mount`,`bytes0`,`bytes1`,`bytes2`,`emote`,`moveflags`,`isActive`,`auras`) VALUES (1550408,0,0,0,0,0,0,0,1,'16380 0');