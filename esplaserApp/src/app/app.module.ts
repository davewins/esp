import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { LaserComponent } from './laser/laser.component';
import { StepperComponent } from './stepper/stepper.component';
import { OriginalstepperComponent } from './originalstepper/originalstepper.component';
import { LibrariesComponent } from './libraries/libraries.component';
import { AdminComponent } from './admin/admin.component';

@NgModule({
  declarations: [
    AppComponent,
    LaserComponent,
    StepperComponent,
    OriginalstepperComponent,
    LibrariesComponent,
    AdminComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
