USE [StoredBase]
GO

/****** Object:  Table [dbo].[ThicknessTable]    Script Date: 12/21/2016 16:51:50 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

SET ANSI_PADDING ON
GO

CREATE TABLE [dbo].[ThicknessTable](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[MinMaxThickness] [varbinary](2400) NULL,
PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]

GO

SET ANSI_PADDING OFF
GO


USE [StoredBase]
GO

/****** Object:  Table [dbo].[ProtectiveThickeningTable]    Script Date: 12/21/2016 16:52:09 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[ProtectiveThickeningTable](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[N0] [int] NULL,
	[N1] [int] NULL,
	[N2] [int] NULL,
	[N3] [int] NULL,
PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]

GO

USE [StoredBase]
GO

/****** Object:  Table [dbo].[StoredThicknessTable]    Script Date: 12/21/2016 16:52:37 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO

CREATE TABLE [dbo].[StoredThicknessTable](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[TubesTableID] [int] NULL,
	[MinThicknessID] [int] NULL,
	[MaxThicknessID] [int] NULL,
	[ProtectiveThickeningTableID] [int] NULL,
PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]

GO



