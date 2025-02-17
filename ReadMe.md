Programın çalışması için izlenecek Adımlar.
1.Inventory dosyası içindeki InventoryDBScript.sql scripti çalıştırılacak.
2.Programa Giriş için Login Bilgileri:

		ADMIN KULLNICISI ICIN
		User Name: admin,
		Password: admin123

		USER(normal) KULLANICI
		User Name :user
		password:user123

Sadece Ekleme olayı Admin rölüne atandı. Diğer tüm durumlara normal kullanıcılar da erişebiliyor.

Neredeyse tüm işlemker için SP Kullanıldı.

Kategori Silinme işleminde kategoriye bağlı ürün varsa silinmesi engelleindi.

Tüm alanlara göre filtreleme eklendi.

DB'de şifreler hashli tutuldu.
Stok seviyesi 5 den düşük olan ürünler excelde raporlandı.


