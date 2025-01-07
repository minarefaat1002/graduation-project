from django.contrib.auth.models import AbstractUser
from django.db import models
from devices.models import Device


class User(AbstractUser):
    email = models.EmailField(unique=True)


class Profile(models.Model):
    user = models.OneToOneField(User, on_delete=models.CASCADE)
    device = models.OneToOneField(Device, on_delete=models.SET_NULL, null=True)

    def __str__(self):
        return str(self.user.username)
