SET SESSION sql_mode='';
SET NAMES utf8mb4;
CREATE TABLE IF NOT EXISTS shared (

	id BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
	nombre VARCHAR(255) NOT NULL,
	descripcion TEXT,
	nReservado INT,
	szReservado TEXT,
	PRIMARY KEY(id),
	INDEX name (nReservado)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS entrega (
	codigo BIGINT NOT NULL,
	documento BIGINT NOT NULL,
	fecha DATETIME,
	PRIMARY KEY(codigo)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS usuario (

	documento BIGINT NOT NULL,
	nombre VARCHAR(255),
	apellido VARCHAR(255),
	PRIMARY KEY(documento)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS foto1 (
	documento BIGINT REFERENCES usuario(documento),
	foto LONGTEXT,
	PRIMARY KEY(documento)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS foto2 (
	documento BIGINT REFERENCES usuario(documento),
	foto LONGTEXT,
	PRIMARY KEY(documento)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS foto3 (
	documento BIGINT REFERENCES usuario(documento),
	foto LONGTEXT,
	PRIMARY KEY(documento)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS huella (
	documento BIGINT REFERENCES usuario(documento),
	ancho INT,
	alto INT,
	resolucion INT,
	huella LONGTEXT,
	PRIMARY KEY(documento)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS template (
	documento BIGINT REFERENCES usuario(documento),	
	huella LONGTEXT,
	PRIMARY KEY(documento)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS propiedades (
	codigo BIGINT REFERENCES entrega(codigo),
	usuario BIGINT REFERENCES shared(id),
	fecha DATETIME,
	PRIMARY KEY(codigo)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS fotoe1 (
	codigo BIGINT REFERENCES entrega(codigo),
	foto LONGTEXT,
	PRIMARY KEY(codigo)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS fotoe2 (
	codigo BIGINT REFERENCES entrega(codigo),
	foto LONGTEXT,
	PRIMARY KEY(codigo)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS fotoe3 (
	codigo BIGINT REFERENCES entrega(codigo),
	foto LONGTEXT,
	PRIMARY KEY(codigo)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS  datosusuario (
	usuario BIGINT NOT NULL REFERENCES usuario(documento),
	tipodocumento INT,
	PRIMARY KEY(usuario)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS entregaguia (
	codigo BIGINT NOT NULL,
	oficina VARCHAR(255),
	ciudad VARCHAR(255),
	fecha DATE,
	usuario BIGINT NOT NULL REFERENCES datosusuario(usuario),
	direccion VARCHAR(255),
	telefono VARCHAR(255),
	cantidad INT,
	peso INT,
	descripcion TEXT,
	observaciones TEXT,
	PRIMARY KEY(codigo),
	INDEX NAME(usuario)
) ENGINE=MyISAM;

CREATE TABLE IF NOT EXISTS fotoentrega (
	codigo BIGINT REFERENCES entregaguia(codigo),
	foto LONGTEXT,
	PRIMARY KEY(codigo)
) ENGINE=MyISAM;

CREATE TABLE grupo (
    id INT PRIMARY KEY AUTO_INCREMENT,
    nombre VARCHAR(120) NOT NULL,
    descripcion VARCHAR(160)
);
CREATE TABLE permisos (
    grupo INT NOT NULL,
    clase VARCHAR(80) NOT NULL,
    permiso INT NOT NULL,
    PRIMARY KEY (grupo, clase)
);
CREATE TABLE usuarios (
    id INT PRIMARY KEY AUTO_INCREMENT,
    login VARCHAR(80) NOT NULL UNIQUE,
    nombre VARCHAR(120) NOT NULL,
    descripcion VARCHAR(160),
    grupo INT NOT NULL,
    vigencia DATETIME NULL,
    expira INT DEFAULT 0,
    ucambio DATETIME NULL,
    acceso DATETIME NULL,
    autonumerico INT DEFAULT 0,
    estado INT DEFAULT 1
);
CREATE TABLE usuariosh (
    usuario INT NOT NULL,
    dia INT NOT NULL,
    entrada INT NOT NULL,
    salida INT NOT NULL,
    PRIMARY KEY (usuario, dia, entrada)
);

INSERT INTO usuario (documento,nombre,apellido) VALUES (900001,'Alicia','Ejemplo');
INSERT INTO entrega (codigo,documento,fecha) VALUES (7301,900001,CURRENT_DATE() + INTERVAL 9 HOUR);
INSERT INTO grupo (id,nombre,descripcion) VALUES (2,'Consulta demo','Grupo ficticio');
INSERT INTO permisos (grupo,clase,permiso) VALUES
 (2,'CInformesView',1),(2,'CSVEView',1);
INSERT INTO usuarios (id,login,nombre,descripcion,grupo) VALUES (401,'consulta_demo','Operador ficticio','0',2);
INSERT INTO shared (id,nombre,descripcion,nReservado,szReservado) VALUES (401,'consulta_demo','0',2,'2');
