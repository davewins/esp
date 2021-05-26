import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { AdminComponent } from './admin/admin.component';
import { OriginalstepperComponent } from './originalstepper/originalstepper.component';
import { StepperComponent } from './stepper/stepper.component';
import { LaserComponent } from './laser/laser.component';
import { LibrariesComponent } from './libraries/libraries.component';



const routes: Routes = [
  { path: 'admin', component: AdminComponent },
  { path: 'originalstepper', component: OriginalstepperComponent },
  { path: 'stepper', component: StepperComponent },
  { path: 'laser', component: LaserComponent },
  { path: 'libraries', component: LibrariesComponent },
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
