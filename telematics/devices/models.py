from django.db import models
import uuid

# Create your models here.


class Device(models.Model):
    device_id = models.UUIDField(primary_key=True, default=uuid.uuid4, editable=False)

    def __str__(self):
        return str(self.device_id)
