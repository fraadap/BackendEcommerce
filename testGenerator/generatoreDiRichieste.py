import random
import richieste

def path():
    path = {}
    path["doLoginUser"] = ["login_user"] # fatto
    path["login_shop"] = ["login_shop"] # fatto
    path["login_admin"] = ["login_admin"] # fatto
    path["doRegisterUser"] = ["registration_user"] # fatto
    path["doRegisterShop"] = ["registration_shop"] # fatto
    path["doRegistrationCourier"] = ["register_courier"] # fatto
    path["addPayment"] = ["users", "", "payment"] # fatto
    path["addMethodPaymentToUser"] = ["users", "", "payment", ""] # fatto
    path["removeMethodPaymentToUser"] = ["users", "", "payment", ""] # fatto
    path["addItem"] = ["shop", "", "item"] # fatto
    path["deleteItem"] = ["users", "", "item", ""] # fatto
    path["unFavoriteItem"] = ["users", "", "item", "", "favorite", ""] # fatto
    path["favoriteItem"] = ["users", "", "item", "", "favorite"] # fatto
    path["cartItem"] = ["users", "", "cart", ""] # fatto
    path["getMyCartPage"] = ["users", "", "cart", ""] # fatto
    path["deleteCart"] = ["users", "", "cart", ""] # fatto
    path["addItemOrderCoureir"] = ["delivery", "", "order", ""] # fatto
    path["addItemOrder"] = ["shop", "", "order", "", "item", ""] # fatto
    path["getMyUserProfile"] = ["users", "", "profile"] # fatto
    path["getMyHomePage"] = ["users", "", "home_page"] # fatto
    path["getMyFavoritePage"] = ["users", "", "favorites"] # fatto
    path["deleteOrder"] = ["users", "", "order", ""] # fatto
    path["getOrderCourier"] = ["users", "", "order"] # fatto
    path["newOrder"] = ["users", "", "order"] # fatto
    path["getOrderShop"] = ["shop", "", "order"] # fatto 
    path["getOrderCourier"] = ["delivery", "", "order"] # fatto
    path["newCategory"] = ["shop", "categories"] # fatto
    path["reviewItem"] = ["users", "", "item", "", "review"]
    path["unReviewItem"] = ["users", "", "item", "", "review", ""]
    return path
    

def genera():
    numero = random.randint(1, 29)
    percorsi = path()
    if numero == 1:
        richieste.doLoginUser("http://localhost:8080/" + percorsi["doLoginUser"][0])
    elif numero == 2:
        richieste.login_shop("http://localhost:8080/" + percorsi["login_shop"][0])
    elif numero == 3:
        richieste.login_admin("http://localhost:8080/" + percorsi["login_admin"][0])
    elif numero == 4:
        richieste.doRegisterUser("http://localhost:8080/" + percorsi["doRegisterUser"][0])
    elif numero == 5:
        richieste.doRegisterShop("http://localhost:8080/" + percorsi["doRegisterShop"][0])
    elif numero == 6:
        richieste.doRegistrationCourier("http://localhost:8080/" + percorsi["doRegistrationCourier"][0])
    elif numero == 7:
        richieste.addPayment("http://localhost:8080/" + percorsi["addPayment"][0] + "/" + str(random.randint(1,10)) + "/" + percorsi["addPayment"][2])
    elif numero == 8:
        richieste.put("http://localhost:8080/" + percorsi["addMethodPaymentToUser"][0] + "/" + str(random.randint(1,10)) + "/" + percorsi["addMethodPaymentToUser"][2] + "/" + "6")
    elif numero == 9:
        richieste.delete("http://localhost:8080/" + percorsi["removeMethodPaymentToUser"][0] + "/" + str(random.randint(11,100)) + "/" + percorsi["removeMethodPaymentToUser"][2] + "/" + "6")
    elif numero == 10:
        richieste.addItem("http://localhost:8080/" + percorsi["addItem"][0] + "/" + str(random.randint(1,1)) + "/" + percorsi["addItem"][2])
    elif numero == 11:
        richieste.delete("http://localhost:8080/" + percorsi["deleteItem"][0] + "/" + str(random.randint(11,100)) + "/" + percorsi["deleteItem"][2])
    elif numero == 12:
        richieste.put("http://localhost:8080/" + percorsi["favoriteItem"][0] + "/" + str(random.randint(1,10)) + "/" + percorsi["favoriteItem"][2] + "/" + str(random.randint(1,10)) + "/" + percorsi["favoriteItem"][4])
    elif numero == 13:
        richieste.delete("http://localhost:8080/" + percorsi["unFavoriteItem"][0] + "/" + str(random.randint(1,10)) + "/" + percorsi["unFavoriteItem"][2] + "/" + str(random.randint(1,10)) + "/" + percorsi["favoriteItem"][4] + "/" + str(random.randint(1,100)))
    elif numero == 14:
        richieste.cartItem("http://localhost:8080/" + percorsi["cartItem"][0] + "/" + str(random.randint(1,10)) + "/" + percorsi["cartItem"][2] + "/" + str(random.randint(1,10)))
    elif numero == 15:
        richieste.get("http://localhost:8080/" + percorsi["getMyCartPage"][0] + "/" + str(random.randint(1,10)) + "/" + percorsi["getMyCartPage"][2] + "/" + str(random.randint(1,10)))
    elif numero == 16:
        richieste.delete("http://localhost:8080/" + percorsi["deleteCart"][0] + "/" + str(random.randint(1,10)) + "/" + percorsi["deleteCart"][2] + "/" + str(random.randint(11,100)))
    elif numero == 17:
        richieste.put("http://localhost:8080/" + percorsi["addItemOrderCoureir"][0] + "/" + str(random.randint(1,10)) + "/" + percorsi["addItemOrderCoureir"][2] + "/" + str(random.randint(1,10)))
    elif numero == 18:
        richieste.newOrder("http://localhost:8080/" + percorsi["newOrder"][0] + "/" + str(random.randint(1,10)) + "/" + percorsi["newOrder"][2])
    elif numero == 19:
        richieste.addItemOrder("http://localhost:8080/" + percorsi["addItemOrder"][0] + "/" + str(random.randint(1,10)) + "/" + percorsi["addItemOrder"][2] + "/" + str(random.randint(1,10)) + "/" + percorsi["addItemOrder"][4] + "/" + str(random.randint(1,10)))
    elif numero == 20:
        richieste.get("http://localhost:8080/" + percorsi["getMyUserProfile"][0] + "/" + str(random.randint(1,10)) + "/" + percorsi["getMyUserProfile"][2])
    elif numero == 21:
        richieste.get("http://localhost:8080/" + percorsi["getMyHomePage"][0] + "/" + str(random.randint(1,10)) + "/" + percorsi["getMyHomePage"][2])
    elif numero == 22:
        richieste.get("http://localhost:8080/" + percorsi["getMyFavoritePage"][0] + "/" + str(random.randint(1,10)) + "/" + percorsi["getMyFavoritePage"][2])
    elif numero == 23:
        richieste.get("http://localhost:8080/" + percorsi["getOrderCourier"][0] + "/" + str(random.randint(1,10)) + "/" + percorsi["getOrderCourier"][2])
    elif numero == 24:
        richieste.get("http://localhost:8080/" + percorsi["getOrderShop"][0] + "/" + str(random.randint(1,10)) + "/" + percorsi["getOrderShop"][2])
    elif numero == 25:
        richieste.get("http://localhost:8080/" + percorsi["getOrderCourier"][0] + "/" + str(random.randint(1,10)) + "/" + percorsi["getOrderCourier"][2])
    elif numero == 26:
        richieste.delete("http://localhost:8080/" + percorsi["deleteOrder"][0] + "/" + str(random.randint(1,10)) + "/" + percorsi["deleteOrder"][2] + "/" + str(random.randint(1, 10)))
    elif numero == 27:
        richieste.newCategory("http://localhost:8080/" + percorsi["newCategory"][0] + "/" + percorsi["newCategory"][1])
    elif numero == 28:
        richieste.reviewItem("http://localhost:8080/" + percorsi["reviewItem"][0] + "/" + str(random.randint(1,10)) + "/" + percorsi["reviewItem"][2] + "/" + str(random.randint(1,10)) + "/" + percorsi["reviewItem"][4])
    elif numero == 29:
        richieste.delete("http://localhost:8080/" + percorsi["unReviewItem"][0] + "/" + str(random.randint(1,10)) + "/" + percorsi["unReviewItem"][2] + "/" + str(random.randint(1,10)) + "/" + percorsi["unReviewItem"][4] + "/" + str(random.randint(1,10)))
    print()
        