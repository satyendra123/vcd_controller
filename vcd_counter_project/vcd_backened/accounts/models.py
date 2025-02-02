from django.contrib.auth.models import AbstractBaseUser, BaseUserManager
from django.db import models

class CustomUserManager(BaseUserManager):
    def create_user(self, username, password=None, **extra_fields):
        if not username:
            raise ValueError('The Username field must be set')
        user = self.model(username=username, **extra_fields)
        user.set_password(password)
        user.save(using=self._db)
        return user

    def create_superuser(self, username, password=None, **extra_fields):
        return self.create_user(username, password, **extra_fields)

class CustomUser(AbstractBaseUser):
    id = models.AutoField(primary_key=True)
    name = models.CharField(max_length=255)
    username = models.CharField(unique=True, max_length=255)
    password = models.CharField(max_length=255)
    is_active = models.BooleanField(default=True)

    # Removing the `last_login` field by explicitly overriding it to None
    last_login = None

    objects = CustomUserManager()

    USERNAME_FIELD = 'username'
    REQUIRED_FIELDS = ['name']

    class Meta:
        db_table = 'custom_users'

    def __str__(self):
        return self.username
