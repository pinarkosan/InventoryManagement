CREATE DATABASE InventoryDB;
GO

USE [InventoryDB]
GO
/****** Object:  Table [dbo].[Categories]    Script Date: 2/18/2025 12:45:24 AM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Categories](
	[CategoryID] [int] IDENTITY(1,1) NOT NULL,
	[CategoryName] [nvarchar](50) NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[CategoryID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Items]    Script Date: 2/18/2025 12:45:24 AM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Items](
	[ItemID] [int] IDENTITY(1,1) NOT NULL,
	[ItemName] [nvarchar](100) NOT NULL,
	[CategoryID] [int] NULL,
	[Quantity] [int] NOT NULL,
	[UnitPrice] [decimal](10, 2) NOT NULL,
	[LastUpdated] [datetime] NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[ItemID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Users]    Script Date: 2/18/2025 12:45:24 AM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Users](
	[UserID] [int] IDENTITY(1,1) NOT NULL,
	[Username] [nvarchar](50) NOT NULL,
	[Password] [varbinary](64) NOT NULL,
	[Role] [nvarchar](50) NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[UserID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
ALTER TABLE [dbo].[Items] ADD  DEFAULT (getdate()) FOR [LastUpdated]
GO
ALTER TABLE [dbo].[Items]  WITH CHECK ADD FOREIGN KEY([CategoryID])
REFERENCES [dbo].[Categories] ([CategoryID])
GO
ALTER TABLE [dbo].[Items]  WITH CHECK ADD  CONSTRAINT [FK_Items_Categories] FOREIGN KEY([CategoryID])
REFERENCES [dbo].[Categories] ([CategoryID])
ON UPDATE CASCADE
ON DELETE CASCADE
GO
ALTER TABLE [dbo].[Items] CHECK CONSTRAINT [FK_Items_Categories]
GO
/****** Object:  StoredProcedure [dbo].[sp_AddCategory]    Script Date: 2/18/2025 12:45:24 AM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- 📌 Kategori Ekleme SP (Geliştirilmiş Versiyon)
CREATE PROCEDURE [dbo].[sp_AddCategory]
    @CategoryName NVARCHAR(50),
    @NewCategoryID INT OUTPUT  -- 📌 Yeni eklenen CategoryID'yi geri döndürmek için
AS
BEGIN
    SET NOCOUNT ON;

    -- 📌 Aynı isimde kategori var mı kontrol et
    IF EXISTS (SELECT 1 FROM Categories WHERE CategoryName = @CategoryName)
    BEGIN
        RAISERROR ('Bu kategori zaten mevcut!', 16, 1);
        RETURN;
    END

    -- 📌 Yeni kategoriyi ekle
    INSERT INTO Categories (CategoryName)
    VALUES (@CategoryName);

    -- 📌 Yeni eklenen ID'yi döndür
    SET @NewCategoryID = SCOPE_IDENTITY();

    -- 📌 Başarı durumunu döndürmek için bir sonuç seti döndür
    SELECT @NewCategoryID AS CategoryID;
END;
GO
/****** Object:  StoredProcedure [dbo].[sp_AddItem]    Script Date: 2/18/2025 12:45:24 AM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- 📌 Ürün Ekleme SP
CREATE PROCEDURE [dbo].[sp_AddItem]
    @ItemName NVARCHAR(100),
    @CategoryID INT,
    @Quantity INT,
    @UnitPrice DECIMAL(10,2)
AS
BEGIN
    BEGIN TRY
        BEGIN TRANSACTION;  -- 📌 Transaction başlat
        
        INSERT INTO Items (ItemName, CategoryID, Quantity, UnitPrice, LastUpdated)
        VALUES (@ItemName, @CategoryID, @Quantity, @UnitPrice, GETDATE());

        COMMIT TRANSACTION; -- 📌 İşlemi onayla
    END TRY
    BEGIN CATCH
        ROLLBACK TRANSACTION; -- 📌 Hata olursa işlemi geri al
        THROW; -- 📌 Hata fırlat
    END CATCH
END;

GO
/****** Object:  StoredProcedure [dbo].[sp_AuthenticateUser]    Script Date: 2/18/2025 12:45:24 AM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[sp_AuthenticateUser]
    @Username NVARCHAR(50),
    @Password NVARCHAR(256)
AS
BEGIN
    SET NOCOUNT ON;

    -- Kullanıcı adı ve şifre eşleşmesini kontrol et
SELECT Role 
FROM Users 
WHERE Username = @Username 
AND Password = CONVERT(VARBINARY(64), @Password, 2);


END
GO
/****** Object:  StoredProcedure [dbo].[sp_DeleteCategory]    Script Date: 2/18/2025 12:45:24 AM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- 📌 Kategori Silme SP
CREATE PROCEDURE [dbo].[sp_DeleteCategory]
    @CategoryID INT
AS
BEGIN
    SET NOCOUNT ON;
    
    BEGIN TRY
        BEGIN TRANSACTION;  -- 📌 Transaction başlat

        -- 📌 Önce bu kategoriye bağlı ürün olup olmadığını kontrol et
        IF EXISTS (SELECT 1 FROM Items WHERE CategoryID = @CategoryID)
        BEGIN
            RAISERROR ('Cannot delete category. It is linked to items.', 16, 1);
            ROLLBACK TRANSACTION;  -- 📌 Hata varsa işlemi geri al
            RETURN;
        END

        -- 📌 Kategoriyi sil
        DELETE FROM Categories WHERE CategoryID = @CategoryID;

        -- 📌 Silme işlemi başarılı oldu mu kontrol et
        IF @@ROWCOUNT > 0
        BEGIN
            COMMIT TRANSACTION;  -- 📌 İşlemi onayla
            SELECT 'Success' AS Result;
        END
        ELSE
        BEGIN
            ROLLBACK TRANSACTION;  -- 📌 Silme başarısızsa geri al
            RAISERROR ('Category not found.', 16, 1);
        END

    END TRY
    BEGIN CATCH
        ROLLBACK TRANSACTION;  -- 📌 Herhangi bir hata olursa işlemi geri al
        DECLARE @ErrorMessage NVARCHAR(4000), @ErrorSeverity INT, @ErrorState INT;
        SELECT @ErrorMessage = ERROR_MESSAGE(), @ErrorSeverity = ERROR_SEVERITY(), @ErrorState = ERROR_STATE();
        RAISERROR (@ErrorMessage, @ErrorSeverity, @ErrorState);
    END CATCH
END;

GO
/****** Object:  StoredProcedure [dbo].[sp_DeleteItem]    Script Date: 2/18/2025 12:45:24 AM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- 📌 Ürün Silme SP
CREATE PROCEDURE [dbo].[sp_DeleteItem]
    @ItemID INT
AS
BEGIN
    DELETE FROM Items WHERE ItemID = @ItemID;

    -- ✅ Her zaman bir sonuç döndür
    IF @@ROWCOUNT > 0
    BEGIN
        SELECT 'Success' AS Result;
    END
    ELSE
    BEGIN
        SELECT 'No row deleted' AS Result;
    END
END;
GO
/****** Object:  StoredProcedure [dbo].[sp_GetAllCategories]    Script Date: 2/18/2025 12:45:24 AM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- 📌 Tüm Kategorileri Getirme SP
CREATE PROCEDURE [dbo].[sp_GetAllCategories]
AS
BEGIN
    SELECT * FROM Categories;
END;
GO
/****** Object:  StoredProcedure [dbo].[sp_GetAllItems]    Script Date: 2/18/2025 12:45:24 AM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- 📌 Tüm Ürünleri Getirme SP
CREATE PROCEDURE [dbo].[sp_GetAllItems]
AS
BEGIN
    SELECT 
        i.ItemID, 
        i.ItemName, 
        c.CategoryName,  -- 📌 `CategoryID` yerine `CategoryName` getiriyoruz
		i.CategoryID,
        i.Quantity, 
        i.UnitPrice, 
        i.LastUpdated
    FROM Items i
    INNER JOIN Categories c ON i.CategoryID = c.CategoryID;  -- 📌 Join ile kategori ismini alıyoruz
END;
GO
/****** Object:  StoredProcedure [dbo].[sp_GetLowStockItems]    Script Date: 2/18/2025 12:45:24 AM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- 📌 Düşük Stok Raporu SP (5 adetten az olan ürünleri listeler)
CREATE PROCEDURE [dbo].[sp_GetLowStockItems]
AS
BEGIN
    SELECT * FROM Items WHERE Quantity < 5;
END;
GO
/****** Object:  StoredProcedure [dbo].[sp_UpdateCategory]    Script Date: 2/18/2025 12:45:24 AM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[sp_UpdateCategory]
    @CategoryID INT,
    @NewCategoryName NVARCHAR(50)
AS
BEGIN
    SET NOCOUNT ON;

    -- 📌 Kategori adını güncelle
    UPDATE Categories 
    SET CategoryName = @NewCategoryName
    WHERE CategoryID = @CategoryID;

    -- 📌 Güncelleme başarılı olduysa, sonucu döndür
    IF @@ROWCOUNT > 0
    BEGIN
        SELECT 'Success' AS Result;
    END
END
GO
/****** Object:  StoredProcedure [dbo].[sp_UpdateItem]    Script Date: 2/18/2025 12:45:24 AM ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- 📌 Ürün Güncelleme SP
CREATE PROCEDURE [dbo].[sp_UpdateItem]
    @ItemID INT,
    @ItemName NVARCHAR(100),
    @CategoryID INT,
    @Quantity INT,
    @UnitPrice DECIMAL(10,2)
AS
BEGIN
    UPDATE Items
    SET 
        ItemName = @ItemName,
        CategoryID = @CategoryID,
        Quantity = @Quantity,
        UnitPrice = @UnitPrice,
        LastUpdated = GETDATE()
    WHERE ItemID = @ItemID;

    -- Eğer güncelleme olduysa sonucu döndür
    IF @@ROWCOUNT > 0
    BEGIN
        SELECT 'Success' AS Result;
    END
    ELSE
    BEGIN
        SELECT 'No update performed' AS Result;
    END
END;
GO

-- Kullanıcıyı tabloya eklemek
INSERT INTO Users (Username, Password, Role) 
VALUES ('admin', HASHBYTES('SHA2_256', 'admin123'), 'Admin');
GO
-- Kullanıcıyı tabloya eklemek
INSERT INTO Users (Username, Password, Role) 
VALUES ('user', HASHBYTES('SHA2_256', 'user123'), 'User');
GO
