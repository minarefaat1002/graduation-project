from . import views
from django.urls import path


urlpatterns = [
    path("auth/", views.auth, name="auth"),
    path("login/", views.login, name="login"),
]
