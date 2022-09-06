DELETE FROM `creature_template_movement` WHERE `CreatureId` = 8964;
INSERT INTO `creature_template_movement` (`CreatureId`, `Ground`, `Swim`, `Flight`, `Rooted`, `Chase`, `Random`, `InteractionPauseTimer`) VALUES (8964, 1, 0, 2, 0, 0, 0, NULL); -- Sets the drakes to be flying.

DELETE FROM `waypoint_data` WHERE `id` = 33430;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES (33430, 1, -7526.11, -1065.07, 290.051, NULL, 0, 2, 0, 100, 0), -- Perimeter drake
(33430, 2, -7528.77, -1060.98, 289.801, NULL, 0, 2, 0, 100, 0),
(33430, 3, -7554.44, -1032.65, 293.967, NULL, 0, 2, 0, 100, 0),
(33430, 4, -7569.05, -1024.16, 297.773, NULL, 0, 2, 0, 100, 0),
(33430, 5, -7603.77, -1019.19, 303.357, NULL, 0, 2, 0, 100, 0),
(33430, 6, -7625.24, -1030.12, 300.606, NULL, 0, 2, 0, 100, 0),
(33430, 7, -7652.77, -1045.97, 291.551, NULL, 0, 2, 0, 100, 0),
(33430, 8, -7670.93, -1065.01, 280.134, NULL, 0, 2, 0, 100, 0),
(33430, 9, -7677.66, -1074.49, 277.884, NULL, 0, 2, 0, 100, 0),
(33430, 10, -7685.64, -1097.26, 280.412, NULL, 0, 2, 0, 100, 0),
(33430, 11, -7684.97, -1144.9, 293.829, NULL, 0, 2, 0, 100, 0),
(33430, 12, -7666.27, -1177.76, 297.051, NULL, 0, 2, 0, 100, 0),
(33430, 13, -7643.66, -1196.77, 297.273, NULL, 0, 2, 0, 100, 0),
(33430, 14, -7634.54, -1201.61, 295.523, NULL, 0, 2, 0, 100, 0),
(33430, 15, -7597.88, -1211.09, 294.745, NULL, 0, 2, 0, 100, 0),
(33430, 16, -7560.96, -1205.24, 290.357, NULL, 0, 2, 0, 100, 0),
(33430, 17, -7536.04, -1191.71, 287.634, NULL, 0, 2, 0, 100, 0),
(33430, 18, -7519.03, -1173.09, 287.107, NULL, 0, 2, 0, 100, 0),
(33430, 19, -7502.94, -1142.82, 286.44, NULL, 0, 2, 0, 100, 0),
(33430, 20, -7499.3, -1120.66, 286.44, NULL, 0, 2, 0, 100, 0),
(33430, 21, -7502.99, -1091.51, 286.44, NULL, 0, 2, 0, 100, 0),
(33430, 22, -7526.11, -1065.07, 290.051, NULL, 0, 2, 0, 100, 0),
(33430, 23, -7526.11, -1065.07, 290.051, NULL, 2500, 2, 0, 100, 0); -- Again, this delay needs to be parsed, but timing it in 45327 seems to be about 2.5s

DELETE FROM `waypoint_data` WHERE `id` = 33440;
INSERT INTO `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_type`, `action`, `action_chance`, `wpguid`) VALUES (33440, 1, -7591.27, -1118.1, 297.518, NULL, 0, 2, 0, 100, 0), -- Center drake
(33440, 2, -7601.09, -1119.68, 298.604, NULL, 0, 2, 0, 100, 0),
(33440, 3, -7602.51, -1119.91, 298.762, NULL, 0, 2, 0, 100, 0),
(33440, 4, -7623.11, -1121.95, 299.872, NULL, 0, 2, 0, 100, 0),
(33440, 5, -7624.85, -1116.18, 298.984, NULL, 0, 2, 0, 100, 0),
(33440, 6, -7623.08, -1109.36, 301.289, NULL, 0, 2, 0, 100, 0),
(33440, 7, -7621.2, -1101.34, 301.622, NULL, 0, 2, 0, 100, 0),
(33440, 8, -7617.67, -1097.37, 302.956, NULL, 0, 2, 0, 100, 0),
(33440, 9, -7612.52, -1091.61, 305.233, NULL, 0, 2, 0, 100, 0),
(33440, 10, -7603.75, -1088.4, 307.15, NULL, 0, 2, 0, 100, 0),
(33440, 11, -7595.7, -1086.25, 309.706, NULL, 0, 2, 0, 100, 0),
(33440, 12, -7584.76, -1084.86, 313.595, NULL, 0, 2, 0, 100, 0),
(33440, 13, -7573.74, -1089.98, 318.4, NULL, 0, 2, 0, 100, 0),
(33440, 14, -7563.75, -1095.65, 321.122, NULL, 0, 2, 0, 100, 0),
(33440, 15, -7561.29, -1111.07, 323.095, NULL, 0, 2, 0, 100, 0),
(33440, 16, -7562.62, -1134.87, 324.789, NULL, 0, 2, 0, 100, 0),
(33440, 17, -7575.26, -1146.93, 321.928, NULL, 0, 2, 0, 100, 0),
(33440, 18, -7587.25, -1150.97, 322.511, NULL, 0, 2, 0, 100, 0),
(33440, 19, -7599.56, -1147.01, 320.289, NULL, 0, 2, 0, 100, 0),
(33440, 20, -7609.3, -1143.61, 316.206, NULL, 0, 2, 0, 100, 0),
(33440, 21, -7634.01, -1120.37, 312.178, NULL, 0, 2, 0, 100, 0),
(33440, 22, -7622.61, -1101.06, 311.345, NULL, 0, 2, 0, 100, 0),
(33440, 23, -7614.43, -1092.26, 305.678, NULL, 0, 2, 0, 100, 0),
(33440, 24, -7582.06, -1084.94, 298.789, NULL, 0, 2, 0, 100, 0),
(33440, 25, -7562.62, -1112.58, 307.456, NULL, 0, 2, 0, 100, 0),
(33440, 26, -7567.33, -1126.35, 304.345, NULL, 0, 2, 0, 100, 0),
(33440, 27, -7587.61, -1127.06, 299.039, NULL, 0, 2, 0, 100, 0),
(33440, 28, -7595.69, -1122.38, 300.092, NULL, 0, 2, 0, 100, 0),
(33440, 29, -7601.09, -1119.68, 298.604, NULL, 0, 2, 0, 100, 0),
(33440, 30, -7601.09, -1119.68, 298.604, NULL, 0, 2, 0, 100, 0);
