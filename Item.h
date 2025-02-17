#pragma once
using namespace System;

namespace Models {
    public ref class Item {
    public:
        property int ItemID;
        property String^ ItemName;
        property int CategoryID;
        String^ CategoryName;
        property int Quantity;
        property double UnitPrice;

        Item() {}
        Item(int id, String^ name, int category, int quantity, double price) {
            ItemID = id;
            ItemName = name;
            CategoryID = category;
            Quantity = quantity;
            UnitPrice = price;
        }
    };
}
