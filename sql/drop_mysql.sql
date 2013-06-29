-- ----------------------------------------------------
-- BlizzLikeCore Script to remove MySQL user and tables
-- ----------------------------------------------------

REVOKE ALL PRIVILEGES ON * . * FROM 'blizzlike'@'localhost';

REVOKE ALL PRIVILEGES ON `world` . * FROM 'blizzlike'@'localhost';
REVOKE GRANT OPTION ON `world` . * FROM 'blizzlike'@'localhost';

REVOKE ALL PRIVILEGES ON `characters` . * FROM 'blizzlike'@'localhost';
REVOKE GRANT OPTION ON `characters` . * FROM 'blizzlike'@'localhost';

REVOKE ALL PRIVILEGES ON `auth` . * FROM 'blizzlike'@'localhost';
REVOKE GRANT OPTION ON `auth` . * FROM 'blizzlike'@'localhost';

DROP USER 'blizzlike'@'localhost';

DROP DATABASE IF EXISTS `world` ;
DROP DATABASE IF EXISTS `characters` ;
DROP DATABASE IF EXISTS `auth` ;