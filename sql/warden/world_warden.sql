-- BlizzLikeCore Warden
DELETE FROM `blizzlike_string` WHERE `entry` IN (6617,6618,6619);
INSERT INTO `blizzlike_string` VALUES (6617,'|cfff00000Account [%s] was banned by warden anticheat.|r',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(6618,'|cfff00000Account [%s] was kicked by warden anticheat.|r',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(6619,'|cfff00000Account [%s] was detected by warden anticheat.|r',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);

--
-- Table structure for table `warden_data_result`
--

DROP TABLE IF EXISTS `warden_data_result`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `warden_data_result` (
  `id` int(4) NOT NULL AUTO_INCREMENT,
  `check` int(3) DEFAULT NULL,
  `data` tinytext,
  `str` tinytext,
  `address` int(8) DEFAULT NULL,
  `length` int(2) DEFAULT NULL,
  `result` tinytext,
  `comment` text,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1014 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `warden_data_result`
--

LOCK TABLES `warden_data_result` WRITE;
/*!40000 ALTER TABLE `warden_data_result` DISABLE KEYS */;
INSERT INTO `warden_data_result` VALUES (11,243,'','',8095301,2,'7513','Air Jump 2'),(10,243,'','',9208728,4,'BB8D243F','Wallclimb'),(9,243,'','',6518444,2,'DECA','Wallclimb 1'),(8,243,'','',8095954,3,'8B4908 ','Teleport To Plane 2'),(7,243,'','',8095957,3,'894808 ','Teleport To Plane'),(6,243,'','',8103107,3,'894E3C','No Fall DMG'),(5,243,'','',8100062,2,'7541','Air Jump'),(4,243,'','',8979979,5,'E04D62503F','Hyper Speed'),(3,243,'','',4840352,2,'558B','Lua Protection'),(2,243,'','',9208923,5,'C0854A3340','WoWEmuHacker Detect + Gravity'),(1,243,'','',1342508000,1,'00','WowMaelStrom Detect.');
/*!40000 ALTER TABLE `warden_data_result` ENABLE KEYS */;
UNLOCK TABLES;