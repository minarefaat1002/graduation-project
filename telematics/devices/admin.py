from django.contrib import admin

from .models import Device


# Register your models here.
class DeviceAdmin(admin.ModelAdmin):
    readonly_fields = ("device_id",)


admin.site.register(Device, DeviceAdmin)
